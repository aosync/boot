#ifndef VMAP_VMAP_H
#define VMAP_VMAP_H

#include <stddef.h>
#include <stdint.h>
#include <mem/framer.h>

struct vmap_vmap {
	u64 phoff;

	u64 level;

	u64 *addrs[6];
	u16 idx[6];

	char big;
	u32 bigidx;

	char miss;

	MemFramer *pages;
};
typedef struct vmap_vmap Vmap;

void vmap_init(Vmap *vmap, u64 *base, u64 phoff, u64 level, MemFramer *framer);
void vmap_idx(Vmap *vmap, u64 idx);
u64 vmap_translate(Vmap *vmap, u64 virt);
int vmap_map(Vmap *vmap, u64 virt, u64 phys, u64 size);
void vmap_unmap(Vmap *vmap, u64 virt, u64 size);

#endif