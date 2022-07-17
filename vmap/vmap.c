#include "vmap.h"
#include "errors.h"

#include <rt/man.h>

#include <fmt/fmt.h>


#define PAGESZ 4096
#define BLANK(X) (~((u64)-1 << (X)))

static int memzero(void *buf, size_t n) {
	u8 *dst = buf;

	for (size_t i = 0; i < n; i++)
		if (dst[i] != 0)
			return 0;

	return 1;
}

void vmap_init(Vmap *vmap, u64 *base, u64 phoff, u64 level, MemFramer *framer) {
	*vmap = (Vmap) {
		.phoff = phoff,
		.level = level,
		.pages = framer,
	};

	vmap->addrs[vmap->level] = (u64*)((u64)base + phoff);
}

void vmap_idx(Vmap *vmap, u64 idx) {
	vmap->idx[0] = idx & BLANK(12);
	vmap->bigidx = idx & BLANK(21);
	idx >>= 12;

	for (int i = 1; i < 6; i++) {
		vmap->idx[i] = idx & BLANK(9);
		idx >>= 9;
	}
}

u64 vmap_translate(Vmap *vmap, u64 virt) {
	u64 vaddr = virt;
	vmap_idx(vmap, vaddr);

	for (int i = vmap->level; i > 0; i--) {
		u64 entry = vmap->addrs[i][vmap->idx[i]];

		// Path is not mapped
		if (entry == 0) {
			vmap->miss = 1;
			return 0;
		}

		// Translate in big mode
		if (i == 2 && entry & (1 << 7)) {
			vmap->big = 1;
			entry &= ~BLANK(21);
			i--;
		} else {
			vmap->big = 0;
			entry &= ~BLANK(12);
		}

		vmap->addrs[i - 1] = (void*)(entry + vmap->phoff);
	}

	vmap->miss = 0;
	return (u64)vmap->addrs[0] + (vmap->big ? vmap->bigidx : vmap->idx[0]);
}

static int vmap_map_page0(Vmap *vmap, u64 virt, u64 phys) {
	// Align the respective addresses
	virt &= ~BLANK(12);
	phys &= ~BLANK(12);

	// Update the indices
	vmap_translate(vmap, virt);
	if (!vmap->miss)
		return VMAP_MAPPED;

	for (int i = vmap->level; i > 0; i--) {
		if (vmap->addrs[i] == (u64*)vmap->phoff) {
			u64 addr = (u64)mem_framer_alloc(vmap->pages, 4096);
		
			vmap->addrs[i] = (u64*)(addr + vmap->phoff);
			memset(vmap->addrs[i], 0, 4096);

			if (i < vmap->level)
				vmap->addrs[i + 1][vmap->idx[i + 1]] = addr | 0x3;
		}

		u64 entry = vmap->addrs[i][vmap->idx[i]];
		
		entry &= ~BLANK(12);
		
		vmap->addrs[i - 1] = (u64*)(entry + vmap->phoff);
	}

	vmap->addrs[1][vmap->idx[1]] = phys | 0x3;
	return VMAP_OK;
}

static int vmap_map_page1(Vmap *vmap, u64 virt, u64 phys) {
	// Align the respective addresses
	virt &= ~BLANK(21);
	phys &= ~BLANK(21);

	// Update the indices
	vmap_translate(vmap, virt);
	if (!vmap->miss)
		return VMAP_MAPPED;

	for (int i = vmap->level; i > 1; i--) {
		if (vmap->addrs[i] == (u64*)vmap->phoff) {
			u64 addr = (u64)mem_framer_alloc(vmap->pages, 4096);
			if (addr == 0)
				return VMAP_ALLOC;

			vmap->addrs[i] = (u64*)(addr + vmap->phoff);
			memset(vmap->addrs[i], 0, 4096);

			if (i < vmap->level)
				vmap->addrs[i + 1][vmap->idx[i + 1]] = addr | 0x3;
		}

		u64 entry = vmap->addrs[i][vmap->idx[i]];
		
		entry &= ~BLANK(12);
		
		vmap->addrs[i - 1] = (u64*)(entry + vmap->phoff);
	}

	vmap->addrs[2][vmap->idx[2]] = phys | 0x3 | 1 << 7;
	return VMAP_OK;
}

static int vmap_unmap_page(Vmap *vmap, u64 virt) {
	// Align the respective addresses
	virt &= ~BLANK(12);

	// Update the indices
	vmap_translate(vmap, virt);
	if (vmap->miss)
		return VMAP_OK;

	int start = vmap->big ? 2 : 1;

	for (int i = start; i <= vmap->level; i++) {
		vmap->addrs[i][vmap->idx[i]] = 0;

		if (memzero(vmap->addrs[i], 4096)) {
			u64 l = (u64)vmap->addrs[i] - vmap->phoff;
			mem_framer_free(vmap->pages, (void*)l);
			vmap->addrs[i] = nil;
		} else
			break;
	}

	return VMAP_OK;
}

int vmap_map(Vmap *vmap, u64 virt, u64 phys, u64 size) {
	int err = 0;
	u64 leftv = virt & ~BLANK(12);
	u64 leftp = phys & ~BLANK(12);

	u64 rightv = (virt + size) & ~BLANK(12);

	u64 i = 0;

	for (; leftv + i < rightv && (leftv + i) & BLANK(21); i += (1 << 12))
		err += vmap_map_page0(vmap, leftv + i, leftp + i);
	for (; leftv + i + (1 << 21) <= rightv; i += (1 << 21))
		err += vmap_map_page1(vmap, leftv + i, leftp + i);
	for (; leftv + i < rightv; i += (1 << 12))
		err += vmap_map_page0(vmap, leftv + i, leftp + i);
	
	return err;
}

void vmap_unmap(Vmap *vmap, u64 virt, u64 size) {
	u64 leftv = virt & ~BLANK(12);

	u64 rightv = (virt + size) & ~BLANK(12);

	u64 i = 0;
	for (; leftv + i < rightv && (leftv + i) & BLANK(21); i += (1 << 12))
		vmap_unmap_page(vmap, leftv + i);
	for (; leftv + i + (1 << 21) <= rightv; i += (1 << 21))
		vmap_unmap_page(vmap, leftv + i);
	for (; leftv + i < rightv; i += (1 << 12))
		vmap_unmap_page(vmap, leftv + i);
}