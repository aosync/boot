#include "conv.h"

#include "str.h"

static char convchars[] = {
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'A',
	'B',
	'C',
	'D',
	'E',
	'F'
} ;

// XXX perhaps make these different functions work
// with string views

void str_u64tos(char *dst, u64 i, u8 base) {
	size_t idx = 0;

	if (i == 0)
		dst[idx++] = '0';

	while (i) {
		dst[idx] = convchars[i % base];
		i /= base;
		idx++;
	}

	dst[idx] = '\0';

	str_reverse(dst);
}

// Converts a u64 into its decimal string representation
void str_u64todec(char *dst, u64 i) {
	str_u64tos(dst, i, 10);
}

void str_reverse(char *str) {
	int len = strlen(str);

	for (int i = 0; i < len / 2; i++) {
		char tmp = str[i];
		str[i] = str[len - 1 - i];
		str[len - 1 - i] = tmp;
	}
}