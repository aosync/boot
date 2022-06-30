#include "call.h"
#include "linker.h"
#include "info.h"

#include <biosdrv/mmap.h>
#include <mem/framer.h>
#include <mem/mmap.h>
#include <str/conv.h>

int main();

int off = 0;

// Performs the early initialization quirks 
void bios_quirks() {
	// Install a framer in the upper workspace
	MemFramer *fm = mem_framer_install(&bios_upper_workspace, (void*)0x80000, 4096);
	
	// Allocate the initial mmap block
	void *mmapptr = mem_framer_alloc(fm, 1);
	MemMmapBlk *blk = mem_mmap_install(mmapptr, fm);

	// Mark important defined memory regions
	mem_mmap_mark(blk, 0, 0x400, MEM_MMAP_BIOS_IVT, 0);
	mem_mmap_mark(blk, 0x400, 0x500, MEM_MMAP_BIOS_BDA, 0);
	mem_mmap_mark(blk, 0x500, 0x80000, MEM_MMAP_BOOTLOADER_RECLAIMABLE, 0);
	
	// and the EBDA
	size_t ebda_addr = *((u16*)0x413) * 1024;
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


	// Debug the entries
	short *fb = (short*)0xb8000;

	for (int i = 0; i < 80*25; i++)
		fb[i] = 0;

	MemMmapEntry *entry = (MemMmapEntry*)blk->first;
	while (entry) {

		int i, j;

		char tmp[100];
		str_u64tos(tmp, entry->begin, 16);
		for (i = 0, j = 0; tmp[j]; i++, j++)
			fb[off + i] = tmp[j] | 0xA << 8;
		i++;
		str_u64tos(tmp, entry->end, 16);
		for (j = 0; tmp[j]; i++, j++)
			fb[off + i] = tmp[j] | 0xA << 8;
		i++;
		str_u64tos(tmp, entry->type, 10);
		for (j = 0; tmp[j]; i++, j++)
			fb[off + i] = tmp[j] | 0xA << 8;
		i++;

		off += 80;
		entry = (MemMmapEntry*)entry->next;
	}

	/*int a = 1;
	biosdrv_mmap_init();

	while (biosdrv_mmap_avail()) {
		biosdrv_mmap_cont();
		a++;
	}

	*/

	/*int i, j;

	char tmp[100];
	str_u64tos(tmp, entry->base, 16);
	for (i = 0, j = 0; tmp[j]; i++, j++)
		fb[i] = tmp[j] | 0xA << 8;
	i++;
	str_u64tos(tmp, entry->base + entry->length, 16);
	for (j = 0; tmp[j]; i++, j++)
		fb[i] = tmp[j] | 0xA << 8;
	i++;
	str_u64tos(tmp, entry->type, 16);
	for (j = 0; tmp[j]; i++, j++)
		fb[i] = tmp[j] | 0xA << 8;
	i++;
	*/
}