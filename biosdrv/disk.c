#include "disk.h"

#include <stdint.h>

#include <bios/call.h>
#include <bios/info.h>
#include <bios/linker.h>

static char biosdrv_disk_ext = 0;

char biosdrv_disk_init() {
	bios_regs.eax = 0x41 << 8;
	bios_regs.edx = bios_info.bootdrive.number;
	bios_regs.ebx = 0x55AA;
	bios_call(0x13);

	if (bios_regs.cf == 0 && bios_regs.ebx == 0xAA55)
		biosdrv_disk_ext = 1;
	else
		biosdrv_disk_ext = 0;
	
	return biosdrv_disk_ext;
}

static char biosdrv_fill_workspace_chs(off_t off) {
	u64 addr = (u64)&bios_lower_workspace;
	bios_regs.ebx = addr % 16;
	bios_regs.es = addr / 16;

	u64 lba = off / 512;

	u64 temp = lba / bios_info.bootdrive.sector_per_track;
	u64 sector = (lba % bios_info.bootdrive.sector_per_track) + 1;
	u64 head = temp % bios_info.bootdrive.heads;
	u64 cylinder = temp / bios_info.bootdrive.heads;

	bios_regs.eax = 8 | 0x2 << 8;
	bios_regs.ecx = sector | cylinder << 8;
	bios_regs.edx = bios_info.bootdrive.number | head << 8;
	bios_call(0x13);

	return bios_regs.cf == 0;
}

struct biosdrv_dap {
	u8 size;
	u8 rsvd;
	u16 sectors;
	u32 dst;
	u64 off;
}__attribute__((packed));
typedef struct biosdrv_dap BiosDrvDap;

static char biosdrv_fill_workspace_lba(off_t off) {
	u64 addr = (u64)&bios_lower_workspace;
	BiosDrvDap dap = (BiosDrvDap) {
		.size = 0x10,
		.sectors = 8,
		.dst = ((addr / 16) << 16) | (addr % 16),
		.off = off / 512
	};

	bios_regs.eax = 0x42 << 8;
	bios_regs.edx = bios_info.bootdrive.number;
	bios_regs.esi = (u64)&dap;
	bios_call(0x13);

	return bios_regs.cf == 0;
}

static char biosdrv_fill_workspace(off_t off) {
	if (biosdrv_disk_ext)
		return biosdrv_fill_workspace_lba(off);
	else
		return biosdrv_fill_workspace_chs(off);
}

ssize_t biosdrv_disk_read(IoFile *file, void *buf, size_t n, off_t off) {
	u8 *dst = buf;
	u8 *src = &bios_lower_workspace;

	ssize_t count = 0;

	while (count < n) {
		off_t head = (off + count) % 512;
		if (!biosdrv_fill_workspace(off + count))
			return -1;
		while (count < n && head < 4096) {
			dst[count] = src[head];
			count++;
			head++;
		}
	}

	return count;
}