#include "map.h"

#define BLKSZ(X) (1 << (X))
#define BLANK(X) (~((u64)-1 << (X)))

u64 bs_map_translate(BsMap *map, size_t addr) {
	int highest = bs_map_idx(map, addr);
	map->invalidate = 0;

	// Find the highest level
	u8 level = 0;
	for (int i = 0; i < 8; i++)
		if (map->idx[i])
			level = i;
		
	if (level > map->level)
		return 0;
	
	u16 size = BLKSZ(map->blksz);
	u64 next = map->addrs[highest];

	for (int i = highest; i > 0; i--) {
		map->addrs[i] = next;

		u64 loc = next * size + map->idx[i] * 8;
		io_pread(map->file, &next, 8, loc);

		if (next == 0)
			return 0;
	}

	map->addrs[0] = next;
	return next * size + map->idx[0];
}

i8 bs_map_idx(BsMap *map, u64 addr) {
	u8 newidx;

	i8 highest = -1;

	newidx = addr & BLANK(map->blksz);
	if (newidx != map->idx[0])
		highest = 0;
	map->idx[0] = newidx;
	addr >>= map->blksz;

	for (int i = 1; i < 8; i++) {
		u8 newidx = addr & BLANK(9);

		if (newidx != map->idx[i])
			highest = i;
		
		map->idx[i] = newidx;
		addr >>= 9;
	}

	if (map->invalidate)
		highest = map->level;

	return highest;
}

void bs_map_clear(BsMap *map) {
	for (int i = 0; i < 8; i++)
		map->idx[i] = 0;
	for (int i = 0; i < 8; i++)
		map->addrs[i] = 0;
	
	map->addrs[map->level] = map->base;
	map->invalidate = 1;
}