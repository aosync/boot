#include "allocator.h"

#include <stddef.h>

void *mem_allocator_alloc(MemAllocator *self, size_t n) {
	if (self->alloc)
		return self->alloc(self, n);

	return nil;
}

void *mem_allocator_realloc(MemAllocator *self, void *blk, size_t n) {
	if (self->realloc)
		return self->realloc(self, blk, n);
	
	return nil;
}

void mem_allocator_free(MemAllocator *self, void *blk) {
	if (self->free)
		return self->free(self, blk);
}