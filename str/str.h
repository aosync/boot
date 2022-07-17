#ifndef STR_STR_H
#define STR_STR_H

#include <stdint.h>

char *str_split(char *st, char c, int *a);

size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int streq(const char *s1, const char *s2);

#endif