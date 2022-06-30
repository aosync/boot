#ifndef STD_MAN
#define STD_MAN

#include <stdint.h>

void *memcpy(void *restrict dst, void *restrict src, size_t n);
void *memmove(void *dst, void *src, size_t n);
void *memset(void *s, int c, size_t n);

#endif