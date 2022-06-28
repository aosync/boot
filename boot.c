#include "pmem.h"

int main() {
	palloc_init();
	short *f = pmalloc(80*25*2);

	short *fb = (short*)0xb8000;

	for (int i = 0; i < 80*25; i++)
		fb[i] = 0;

	pfree();
	return 0;
}