#ifndef MEM_FRAMER_H
#define MEM_FRAMER_H

#include <stdint.h>

struct mem_framer_fblk {
	struct mem_framer_fblk *next;
};
typedef struct mem_framer_fblk MemFramerFblk;

// Oneshot physical memory allocator
struct mem_framer {
	struct mem_framer *next;
	size_t blksz;
	MemFramerFblk *fblk;
	void *blk;
	void *end;
};
typedef struct mem_framer MemFramer;

MemFramer *mem_framer_install(void *ptr, void *end, size_t blksz);
void *mem_framer_alloc(MemFramer *self, size_t n);
void mem_framer_free(MemFramer *self, void *blk);

#endif