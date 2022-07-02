#ifndef TEXT_TEXT_H
#define TEXT_TEXT_H

#include <io/file.h>

struct text_text {
	u16 *fb;
	uint x, xlen;
	uint y, ylen;

	u8 attr;
};
typedef struct text_text Text;

void text_init(Text *self, u16 *fb, uint xlen, uint ylen);
void text_clear(Text *self);
void text_set_cursor(Text *self, uint x, uint y);
void text_get_cursor(Text *self, uint *x, uint *y);
void text_set_attr(Text *self, u8 attr);
void text_get_attr(Text *self, u8 *attr);
void text_advance_row(Text *self);
void text_advance_col(Text *self);
void text_putchar(Text *self, char c);

ssize_t text_write(IoFile *file, void *buf, size_t n, off_t off);

#endif