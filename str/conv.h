#ifndef STR_CONV_H
#define STR_CONV_H

#include <stdint.h>

void str_u64tos(char *dst, u64 i, u8 base);
void str_u64todec(char *dst, u64 i);

void str_reverse(char *dst);

#endif