#include "man.h"

void *memcpy(void *restrict dst, void *restrict src, size_t n) {
	u8 *restrict cdst = dst;
	u8 *restrict csrc = src;

	for (size_t i = 0; i < n; i++)
		cdst[i] = csrc[i];
	
	return dst;
}

void *memmove(void *dst, void *src, size_t n) {
	u8 *cdst = dst;
	u8 *csrc = src;

	if (cdst < csrc) {
		size_t i = 0;
		while (i < n) {
			cdst[i] = csrc[i];
			i++;
		}
	} else if (cdst > csrc) {
		size_t i = n;
		while (i-- > 0)
			cdst[i] = csrc[i];
	}

	return dst;
}