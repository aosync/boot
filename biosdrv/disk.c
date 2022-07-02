#include "disk.h"

#include <stdint.h>

#include <bios/call.h>
#include <bios/info.h>
#include <bios/linker.h>

static char biosdrv_fill_workspace(off_t off) {
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