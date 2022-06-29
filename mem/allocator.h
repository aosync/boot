#ifndef MEM_ALLOCATOR_H
#define MEM_ALLOCATOR_H

#include <stdint.h>

// Interface for memory allocation
struct mem_allocator {
	// Allocates a region fitting n bytes of memory.
	// On success, returns a pointer to a region
	// fitting n bytes.
	// On failure, returns nil.
	void *(*alloc)(void* self, size_t);

	// Reallocates a region fitting n bytes of memory
	// from a given region.
	// On success, returns a pointer to a region
	// fitting n bytes.
	// On failure, returns nil.
	void *(*realloc)(void* self, void*, size_t);

	// Frees a region.
	void (*free)(void* self, void*);
};
typedef struct mem_allocator MemAllocator;

void *mem_allocator_alloc(MemAllocator *self, size_t n);
void *mem_allocator_realloc(MemAllocator *self, void *blk, size_t n);
void mem_allocator_free(MemAllocator *self, void *blk);

#endif