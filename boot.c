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
		if (mmap)
			mem_mmap_mark(mmap, phys, phys + 4096, type, 0);
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

	bs.ws = mem_framer_alloc(sys->prim, 4096);

	BsFile root;
	if (bs_root(&bs, &root)) {
		printf("Filesystem has no root!\n");
		return 1;
	}

	BsFile boot;
	if (io_walk((IoFile*)&root, (IoFile*)&boot, "boot")) {
		printf("Filesystem has no /boot directory!\n");
		return 1;
	}

	BsFile boot_ini;
	if (io_walk((IoFile*)&boot, (IoFile*)&boot_ini, "boot.ini")) {
		printf("Filesystem has no /boot/boot.ini!\n");
		return 1;
	}

	Ini ini;
	ini_init(&ini, (IoFile*)&boot_ini);
	char kpath[257];
	if (ini_get(&ini, "kernel", kpath)) {
		printf("boot.ini does not specify a kernel file\n");
		return 1;
	}

	printf("Loading %s...\n", kpath);

	BsFile kernel;
	memcpy(&kernel, &root, sizeof(BsFile));
	if (io_ballad((IoFile*)&kernel, kpath)) {
		printf("Could not walk to %s\n", kpath);
		return 1;
	}

	Elf elf;
	if (elf_init(&elf, (IoFile*)&kernel)) {
		printf("Kernel file is not an ELF executable!\n");
		return 1;
	}

	ElfPhent phent;
	for (int i = 0; elf_phent_get(&elf, &phent, i) == 0; i++) {
		if (phent.type == ELF_PHENT_NULL || phent.memsz == 0)
			continue;

		boot_vmalloc(&vmap, phent.vaddr, phent.memsz, sys->bulk, sys->mmap, MEM_MMAP_KERNEL);
		io_pread((IoFile*)&kernel, (void*)phent.vaddr, phent.filesz, phent.offset);

		printf("phent[%lu].vaddr = %#lx\n", i, phent.vaddr);
	}

	vmap_map(&vmap, 0xffff000000000000, 0, 0x200000000);
	BpBootinfo bootinfo = (BpBootinfo) {
		.phoff = 0xffff000000000000
	};

	void (*kboot)(BpBootinfo*) = (void*)elf.header.entry;
	kboot(&bootinfo);

	printf("Kernel returned to bootloader\n");

	return 0;
}