#ifndef BIOS_INFO_H
#define BIOS_INFO_H

#include <stdint.h>

extern struct {
	struct {
		u8 number;
		u8 sector_per_track;
		u8 heads;
		u16 what;
	}__attribute__((packed)) bootdrive;
	struct {
		u64 base;
		u16 limit;
	}__attribute__((packed)) bioslidtr;
}__attribute__((packed)) bios_info;

#endif