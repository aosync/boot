#ifndef MEM_MMAP_H
#define MEM_MMAP_H

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#include "framer.h"

enum mem_mmap_entry_type {
	MEM_MMAP_FREE = 0,
	MEM_MMAP_USABLE = 1,
	MEM_MMAP_RESERVED = 2,
	MEM_MMAP_ACPI_RECLAIMABLE = 3,
	MEM_MMAP_ACPI_NVS = 4,
	MEM_MMAP_BAD = 5,

	MEM_MMAP_BOOTLOADER_RECLAIMABLE = 1000,
	MEM_MMAP_KERNEL = 2000,

	MEM_MMAP_BIOS_IVT = 3000,
	MEM_MMAP_BIOS_BDA = 3001,
	MEM_MMAP_BIOS_EBDA = 3002,

	MEM_MMAP_UNMARK
};

struct mem_mmap_entry {
	u64 begin;
	u64 end;
	u32 type;
	u32 data;
	u64 next;
}__attribute__((packed));
typedef struct mem_mmap_entry MemMmapEntry;

static_assert(sizeof(MemMmapEntry) == 32, "MemMmapEntry is not 32 bytes long.");

struct mem_mmap_blk {
	u64 nextblk;
	u64 first;
	u64 free; // maybe unused
	MemFramer *framer;
	MemMmapEntry entries[127];
}__attribute__((packed));
typedef struct mem_mmap_blk MemMmapBlk;

static_assert(sizeof(MemMmapBlk) == 4096, "MemMmapBlk is not 4096 bytes long.");

MemMmapBlk *mem_mmap_install(void *ptr, MemFramer *framer);
MemMmapEntry *mem_mmap_alloc(MemMmapBlk *blk);
void mem_mmap_free(MemMmapBlk *blk, MemMmapEntry *entry);
u64 mem_mmap_find_location(MemMmapBlk *blk, u64 begin);
char mem_mmap_fix_overlap(MemMmapBlk *blk);
char mem_mmap_fix_length(MemMmapBlk *blk);
char mem_mmap_collate(MemMmapBlk *blk);
char mem_mmap_collect_zero(MemMmapBlk *blk);
void mem_mmap_mark(MemMmapBlk *blk, u64 begin, u64 end, u32 type, u32 data);

#endif