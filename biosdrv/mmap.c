#include "mmap.h"

#include <bios/linker.h>

BiosDrvMmapEntry *biosdrv_mmap_init() {
	u64 addr = (u64)&bios_lower_workspace;
	bios_regs.edi = addr % 16;
	bios_regs.es = addr / 16;

	bios_regs.eax = 0xE820;
	bios_regs.ebx = 0;
	bios_regs.ecx = 24;
	bios_regs.edx = 0x534D4150;
	bios_call(0x15);

	if (bios_regs.eax != 0x534D4150)
		return nil;
	
	return &bios_lower_workspace;
}

BiosDrvMmapEntry *biosdrv_mmap_cont() {
	u64 addr = (u64)&bios_lower_workspace;
	bios_regs.edi = addr % 16;
	bios_regs.es = addr / 16;

	bios_regs.eax = 0xE820;
	bios_regs.ecx = 24;
	bios_call(0x15);

	if (bios_regs.eax != 0x534D4150)
		return nil;
	
	return &bios_lower_workspace;
}

// Checks whether there are still available entries
char biosdrv_mmap_avail() {
	return bios_regs.ebx != 0 && bios_regs.cf == 0;
}