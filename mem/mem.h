#ifndef MEM_MEM_H
#define MEM_MEM_H

#include "allocator.h"

#include <stdint.h>

extern MemAllocator *std_mem_allocator;

void *alloc(size_t n);
void *realloc(void *blk, size_t n);
void free(void* blk);

#endif