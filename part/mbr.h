#ifndef PART_MBR_H
#define PART_MBR_H

#include <assert.h>

#include <io/io.h>

enum part_mbr_type {
	MBR_TYPE_GPT = 0xEE
};

struct part_mbr_entry {
	u8 attributes;
	char pad0[3];
	u8 type;
	char pad1[3];
	u32 lbegin;
	u32 length;
}__attribute__((packed));
typedef struct part_mbr_entry PartMbrEntry;

struct part_mbr {
	u8 bootstrap[440];
	u32 udid;
	u16 reserved;
	PartMbrEntry entries[4];
	u16 magic;
}__attribute__((packed));
typedef struct part_mbr PartMbr;

static_assert(sizeof(PartMbr) == 512, "Mbr is not 512 bytes.");

char part_mbr_init(PartMbr *self, IoFile *file);

#endif