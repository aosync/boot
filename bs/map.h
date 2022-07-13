#ifndef BS_MAP_H
#define BS_MAP_H

#include <stdint.h>
#include <io/io.h>

struct bs_map {
	IoFile *file;
	u64 addrs[8];
	u16 idx[8];
	u16 blksz;
	u8 level;
	u64 base;
	u8 invalidate;
};
typedef struct bs_map BsMap;

u64 bs_map_translate(BsMap *map, size_t addr);
i8 bs_map_idx(BsMap *map, u64 addr);
void bs_map_clear(BsMap *map);

#endif