#include "call.h"
#include "linker.h"
#include "info.h"

#include <biosdrv/disk.h>
#include <biosdrv/mmap.h>
#include <mem/framer.h>
#include <mem/mmap.h>
#include <str/conv.h>
#include <sys/sys.h>
#include <text/text.h>
#include <io/io.h>

#define BIOS_LIMIT ((void*)0x80000000)
#define BIOS_EBDA (*((volatile u16*)0x413) * 1024)

int boot(Sys *sys);

int off = 0;

// Performs the early initialization quirks 
void bios_quirks() {
	// Install a framer in the upper workspace
	MemFramer *prim = mem_framer_install(&bios_upper_workspace, (void*)0x80000, 4096);
	
	// Allocate the initial mmap block
	void *mmapptr = mem_framer_alloc(prim, 1);
	MemMmapBlk *blk = mem_mmap_install(mmapptr, prim);

	// Mark important defined memory regions
	mem_mmap_mark(blk, 0, 0x400, MEM_MMAP_BIOS_IVT, 0);
	mem_mmap_mark(blk, 0x400, 0x500, MEM_MMAP_BIOS_BDA, 0);
	mem_mmap_mark(blk, 0x500, 0x80000, MEM_MMAP_BOOTLOADER_RECLAIMABLE, 0);
	
	// and the EBDA
	size_t ebda_addr = BIOS_EBDA;
	mem_mmap_mark(blk, ebda_addr, ebda_addr + 128 * 1024, MEM_MMAP_BIOS_EBDA, 0);

	// Add the E820 memory entries
	BiosDrvMmapEntry *bentry = biosdrv_mmap_init();
	mem_mmap_mark(blk, bentry->base, bentry->base + bentry->length, bentry->type, bentry->acpi_attr);

	while (biosdrv_mmap_avail()) {
		bentry = biosdrv_mmap_cont();

		// Skip invalid entry
		if (bentry->length == 0)
			continue;

		mem_mmap_mark(blk, bentry->base, bentry->base + bentry->length, bentry->type, bentry->acpi_attr);
	}

	MemFramer *bulk = nil;
	MemMmapEntry *entry;
	for (entry = (MemMmapEntry*)blk->first; entry; entry = (MemMmapEntry*)entry->next) {
		if (entry->type != MEM_MMAP_USABLE)
			continue;

		if (entry->begin >= (u64)BIOS_LIMIT)
			continue;
		
		void *end = (void*)entry->end;
		if (end >= BIOS_LIMIT)
			end = BIOS_LIMIT;

		MemFramer *tmp = mem_framer_install((void*)entry->begin, end, 4096);
		if (tmp == nil)
			continue;

		if (bulk)
			mem_framer_append(bulk, tmp);
		else
			bulk = tmp;
	}

	IoFile disk = (IoFile) {
		.read = biosdrv_disk_read
	};

	Text text;
	text_init(&text, (void*)0xB8000, 80, 25);
	text_clear(&text);
	IoFile cons = (IoFile) {
		.inner = &text,
		.write = text_write
	};

	io_stdout = &cons;

	Sys sys = (Sys) {
		.prim = prim,
		.bulk = bulk,
		.mmap = blk,
		.disk = &disk,
		.cons = &cons,
	};

	boot(&sys);
}