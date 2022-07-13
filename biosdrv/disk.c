#include "biosdrv.h"
#include "disk.h"

#include <stdint.h>

#include <bios/call.h>
#include <bios/info.h>
#include <bios/linker.h>

// The size of a sector
#define SECTOR_SIZE 512

// Amount of sectors in the lower workspace (the workspace is 4096 bytes long)
#define WS_SECTORS 8
#define WS_BYTES 4096

static char biosdrv_disk_ext = 0;
static u64 last_lba = 0;

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

static char biosdrv_fill_workspace_chs(u64 lba) {
	u64 addr = (u64)&bios_lower_workspace;
	bios_regs.ebx = addr % 16;
	bios_regs.es = addr / 16;

	u64 temp = lba / bios_info.bootdrive.sector_per_track;
	u64 sector = (lba % bios_info.bootdrive.sector_per_track) + 1;
	u64 head = temp % bios_info.bootdrive.heads;
	u64 cylinder = temp / bios_info.bootdrive.heads;

	bios_regs.eax = WS_SECTORS | 0x2 << 8;
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

static char biosdrv_fill_workspace_lba(u64 lba) {
	u64 addr = (u64)&bios_lower_workspace;
	BiosDrvDap dap = (BiosDrvDap) {
		.size = 0x10,
		.sectors = WS_SECTORS,
		.dst = ((addr / 16) << 16) | (addr % 16),
		.off = lba
	};

	bios_regs.eax = 0x42 << 8;
	bios_regs.edx = bios_info.bootdrive.number;
	bios_regs.esi = (u64)&dap;
	bios_call(0x13);

	return bios_regs.cf == 0;
}

static u64 biosdrv_fill_workspace(off_t off, char *status) {
	u64 lba = off / SECTOR_SIZE;

	// Do not refill the workspace off is already 
	if (biosdrv_ws_owner == BIOSDRV_DISK_ID && lba >= last_lba && lba < last_lba + WS_SECTORS)
		return last_lba;

	char rc;
	if (biosdrv_disk_ext)
		rc = biosdrv_fill_workspace_lba(lba);
	else
		rc = biosdrv_fill_workspace_chs(lba);

	if (rc) {
		biosdrv_ws_owner = BIOSDRV_DISK_ID;
		last_lba = lba;
	} else
		biosdrv_ws_owner = 0x0;

	if (status)
		*status = rc;

	return lba;
}

ssize_t biosdrv_disk_read(IoFile *file, void *buf, size_t n, off_t off) {
	u8 *dst = buf;
	u8 *src = &bios_lower_workspace;

	ssize_t count = 0;

	while (count < n) {
		char status = 1;
		u64 lba = biosdrv_fill_workspace(off + count, &status);
		if (!status)
			return -1;

		off_t head = off + count - lba * SECTOR_SIZE;
		while (count < n && head < WS_BYTES) {
			dst[count] = src[head];
			count++;
			head++;
		}
	}

	return count;
}