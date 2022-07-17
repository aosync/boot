#include "str.h"

#include <stddef.h>

char *str_split(char *s, char c, int *a) {
	if (*a < 0) {
		*a = 0;
		return nil;
	} else if (!s[*a]) {
		s[*a] = c;
		(*a)++;
	}

	int init = *a;

	for (; s[*a] && s[*a] != c; (*a)++);

	if (s[*a] == c) {
		s[*a] = '\0';
		return s + init;
	}

	*a = -1;
	return s + init;
}

// Calculates the length of the string
size_t strlen(const char *s) {
	size_t len = 0;
	for (; s[len]; len++);
	return len;
}

// Compares two strings:
// Returns 0 if they are equal
// Returns s1[l] - s2[l] if they are not equal
// where l is the index of divergence
int strcmp(const char *s1, const char *s2) {
	size_t cursor = 0;

	int diff = s1[cursor] - s2[cursor];
	while (s1[cursor] && s2[cursor] && diff == 0) {
		cursor++;
		diff = s1[cursor] - s2[cursor];
	}
	
	return diff;
}

// Checks if two strings are equal: true if yes
// false if not.
int streq(const char *s1, const char *s2) {
	return strcmp(s1, s2) == 0;
}