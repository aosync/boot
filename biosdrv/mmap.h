#ifndef BIOSDRV_MMAP_H
#define BIOSDRV_MMAP_H

#include <stddef.h>
#include <stdint.h>

#include <bios/call.h>

#define BIOSDRV_MMAP_ID 0x676C83E1F34B302E

enum biosdrv_mmap_entry_type {
	BIOSDRV_MMAP_USABLE = 1,
	BIOSDRV_MMAP_RESERVED = 2,
	BIOSDRV_MMAP_ACPI_RECLAIMABLE = 3,
	BIOSDRV_MMAP_ACPI_NVS = 4,
	BIOSDRV_MMAP_BAD = 5
};

struct biosdrv_mmap_entry {
	u64 base;
	u64 length;
	u32 type;
	u32 acpi_attr;
}__attribute__((packed));
typedef struct biosdrv_mmap_entry BiosDrvMmapEntry;

BiosDrvMmapEntry *biosdrv_mmap_init(void);
BiosDrvMmapEntry *biosdrv_mmap_cont(void);
char biosdrv_mmap_avail(void);

#endif