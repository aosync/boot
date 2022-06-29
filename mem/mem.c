#include "mem.h"

#include <stddef.h>

// The standard allocator
MemAllocator *std_mem_allocator = nil;

void *alloc(size_t n) {
	if (std_mem_allocator)
		return mem_allocator_alloc(std_mem_allocator, n);
	
	return nil;
}

void *realloc(void *blk, size_t n) {
	if (std_mem_allocator)
		return mem_allocator_realloc(std_mem_allocator, blk, n);

	return nil;
}

void free(void *blk) {
	if (std_mem_allocator)
		mem_allocator_free(std_mem_allocator, blk);
}