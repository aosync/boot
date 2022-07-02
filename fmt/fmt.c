#include "fmt.h"

#include <stddef.h>
#include <man.h>
#include <io/io.h>
#include <str/str.h>
#include <str/conv.h>

static void padder(char *s, int padding) {
	int len = strlen(s);
	int dis = padding - len;
	if (dis < 0)
		return;
	memmove(s + dis, s, len + 1);
	memset(s, '0', dis);
}

int _fprintf(IoFile *file, char *fmt, va_list ap) {
	char *begin = fmt, *end = fmt;

	int count = 0;

	char escaped = 0;
	char longmode = 0;
	int padding = 0;
	
	// Variables used in the switch statement
	char num[64];
	char c;
	char *str;
	int len;

	for (; *end; end++) {
		if (!escaped && *end == '%') {
			count += io_write(file, begin, end - begin);
			begin = end + 1;
			escaped = 1;
			continue;
		}
		
		if (!escaped)
			continue;

		switch (*end) {
		case 'l':
			longmode = 1;
			continue;
		case '0':
			padding = -1;
			continue;
		case '#':
			count += io_write(file, "0x", 2);
			continue;
		case 'c':
			c = va_arg(ap, int);
			count += io_write(file, &c, 1);
			break;
		case 's':
			str = va_arg(ap, char*);
			len = strlen(str);
			count += io_write(file, str, len);
			break;
		case 'p':
			longmode = 1;
			count += io_write(file, "0x", 2);
			/* FALLTHROUGH */
		case 'x':
			if (longmode)
				str_u64tos(num, va_arg(ap, u64), 16);
			else
				str_u64tos(num, va_arg(ap, u32), 16);

			padder(num, padding);

			len = strlen(num);
			count += io_write(file, num, len);
			break;
		case 'd':
			/* FALLTHROUGH */
		case 'u':
			switch (longmode) {
			case 0:
				str_u64todec(num, va_arg(ap, u32));
				break;
			default:
				str_u64todec(num, va_arg(ap, u64));
			}

			padder(num, padding);

			len = strlen(num);
			count += io_write(file, num, len);
			break;
		default:
			if (padding == -1) {
				padding = 0;

				char *b = end;
				for (; *end && *end >= '0' && *end <= '9'; end++);
				char *e = --end;
				for (size_t factor = 1; end >= b; factor *= 10, end--)
					padding += (*end - '0') * factor;
				end = e;
				continue;
			}
		}

		begin = end + 1;
		longmode = 0;
		padding = 0;
		escaped = 0;
	}

	if (begin != end)
		count += io_write(file, begin, end - begin);
	
	return count;
}

int fprintf(IoFile *file, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	int ret = _fprintf(file, fmt, ap);
	va_end(ap);
	return ret;
}

int printf(char *fmt, ...) {
	if (io_stdout == nil)
		return 0;

	va_list ap;
	va_start(ap, fmt);
	int ret = _fprintf(io_stdout, fmt, ap);
	va_end(ap);
	return ret;
}