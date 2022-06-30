#include "framer.h"

#include <stddef.h>

#define ALIGNUP(X, B) (void*)((size_t)(X) + (B) - ((size_t)(X) % (B)))
#define ALIGNDOWN(X, B) (void*)((size_t)(X) - ((size_t)(X) % (B)))

MemFramer *mem_framer_install(void *ptr, void *end, size_t blksz) {
	void *blk = ptr + sizeof(MemFramer);

	if ((size_t)blk % blksz)
		blk = ALIGNUP(blk, blksz);
	
	MemFramer *self = ptr;

	*self = (MemFramer) {
		.next = nil,
		.blksz = blksz,
		.fblk = nil,
		.blk = blk,
		.end = end
	};

	return self;
}

void *mem_framer_alloc(MemFramer *self, size_t n) {
	if (n == 0 || n > self->blksz)
		return nil;

	if (self->fblk) {
		void *ret = self->fblk;
		self->fblk = self->fblk->next;
		return ret;
	}

	void *blk = self->blk;
	void *next_blk = self->blk + self->blksz;

	if (next_blk >= self->end) {
		if (self->next)
			return mem_framer_alloc(self->next, n);
		else
			return nil;
	}

	self->blk = next_blk;

	return blk;
}

void mem_framer_free(MemFramer *self, void *blk) {
	if ((size_t)blk % self->blksz)
		blk = ALIGNDOWN(blk, self->blksz);

	MemFramerFblk *fblk = blk;
	fblk->next = self->fblk;
	self->fblk = fblk;
}