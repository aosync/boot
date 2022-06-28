#include <stdint.h>

extern void *const primitives_stack;

struct palloc_header {
	struct palloc_header *previous;
};

extern struct palloc_header *sp;

void palloc_init(void);
void *pmalloc(size_t n);
void pfree(void);