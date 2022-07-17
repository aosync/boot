#include "bp.h"

extern void kernel_stack_low;

short *fb = (short*)0xB8000;

int boot(BpBootinfo *req) {
	fb[0] = 'K' | 0xD << 8;
	fb[1] = 'E' | 0xD << 8;
	fb[2] = 'R' | 0xD << 8;
	fb[3] = 'N' | 0xD << 8;
	fb[4] = 'E' | 0xD << 8;
	fb[5] = 'L' | 0xD << 8;

	return 0;
}