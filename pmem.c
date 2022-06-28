#include "pmem.h"

struct palloc_header *sp;

// Initializes the palloc system
void palloc_init(void) {
	sp = primitives_stack;
	sp->previous = nil;
}

void *pmalloc(size_t n) {
	struct palloc_header *prev = sp;

	sp = (void*)sp + sizeof(struct palloc_header) + n;
	sp->previous = prev;
	
	return prev + 1;
}

void pfree(void) {
	if (sp->previous == nil)
		return;

	sp = sp->previous;
}