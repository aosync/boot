#include <sys/sys.h>
#include <io/io.h>
#include <str/str.h>

#include <mem/mmap.h>
#include <fmt/fmt.h>
#include <text/text.h>
#include <part/mbr.h>

#include <bs/bs.h>
#include <bios/linker.h>
#include <ini/ini.h>
#include <elf/elf.h>
#include <elf/errors.h>
#include <vmap/vmap.h>

#include <rt/man.h>

#include "bp.h"

char *mmap_entry_type_name(u32 type) {
	switch (type) {
	case MEM_MMAP_USABLE:
		return "USABLE";
	case MEM_MMAP_RESERVED:
		return "RESERVED";
	case MEM_MMAP_ACPI_RECLAIMABLE:
		return "ACPI_RECLAIMABLE";
	case MEM_MMAP_ACPI_NVS:
		return "ACPI_NVS";
	case MEM_MMAP_BAD:
		return "BAD";
	case MEM_MMAP_BOOTLOADER_RECLAIMABLE:
		return "BOOTLOADER_RECLAIMABLE";
	case MEM_MMAP_KERNEL:
		return "KERNEL";
	case MEM_MMAP_BIOS_IVT:
		return "BIOS_IVT";
	case MEM_MMAP_BIOS_BDA:
		return "BIOS_BDA";
	case MEM_MMAP_BIOS_EBDA:
		return "BIOS_EBDA";
	default:
		return "UNKNOWN";
	}
}

void *boot_vmalloc(Vmap *vmap, u64 virt, size_t n, MemFramer *prim, MemMmapBlk *mmap, u32 type) {
	for (size_t count = 0; count < n; count += 4096) {
		vmap_translate(vmap, (u64)virt);
		if (!vmap->miss)
			continue;
		
		u64 phys = (u64)mem_framer_alloc(prim, 4096);
		printf("not marking\n");
		if (mmap) {
			mem_mmap_mark(mmap, phys, phys + 4096, type, 0);
			printf("marking\n");
		}
		vmap_map(vmap, (u64)virt + count, phys, 4096);
	}

	return (void*)virt;
}

int boot() {
	Vmap vmap;
	vmap_init(&vmap, &bios_p4, 0, 4, sys->prim);

	Bs bs;
	if (!bs_init(&bs, sys->disk))
		printf("bsfs successfully initted\n");

	bs.ws = &bios_lower_workspace;

	BsFile root;
	bs_root(&bs, &root);

	BsFile boot;
	io_walk((IoFile*)&root, (IoFile*)&boot, "kernel");

	Elf elf;
	int rc = elf_init(&elf, (IoFile*)&boot);
	if (!rc)
		printf("elf found!\n");
	else {
		printf("elf not found\n");
		return 1;
	}

	ElfPhent phent;
	for (int i = 0; elf_phent_get(&elf, &phent, i) == 0; i++) {
		if (phent.type == ELF_PHENT_NULL || phent.memsz == 0)
			continue;

		boot_vmalloc(&vmap, phent.vaddr, phent.memsz, sys->bulk, sys->mmap, MEM_MMAP_KERNEL);
		io_pread((IoFile*)&boot, (void*)phent.vaddr, phent.filesz, phent.offset);

		printf("phent[%lu].vaddr = %#lx\n", i, phent.vaddr);
	}

	vmap_map(&vmap, 0xffff000000000000, 0, 0x200000000);
	BpBootinfo bootinfo = (BpBootinfo) {
		.phoff = 0xffff000000000000
	};

	void (*kboot)(BpBootinfo*) = (void*)elf.header.entry;
	kboot(&bootinfo);

	printf("kernel aborted\n");

	return 0;
}