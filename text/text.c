#include "text.h"

#include <man.h>

void text_init(Text *self, u16 *fb, uint xlen, uint ylen) {
	*self = (Text) {
		.fb = fb,
		.xlen = xlen,
		.ylen = ylen,
		.attr = 0xA
	};
}

void text_clear(Text *self) {
	uint xo, yo;
	text_get_cursor(self, &xo, &yo);


	for (uint i = 0; i < self->xlen; i++)
		for (uint j = 0; j < self->ylen; j++) {
			text_set_cursor(self, i, j);
			text_putchar(self, ' ');
		}
	
	text_set_cursor(self, xo, yo);
}

void text_set_cursor(Text *self, uint x, uint y) {
	self->x = x;
	self->y = y;
}
void text_get_cursor(Text *self, uint *x, uint *y) {
	if (x)
		*x = self->x;
	if (y)
		*y = self->y;
}
void text_set_attr(Text *self, u8 attr) {
	self->attr = attr;
}
void text_get_attr(Text *self, u8 *attr) {
	if (attr)
		*attr = self->attr;
}
void text_advance_row(Text *self) {
	self->y++;
	if (self->y >= self->ylen) {
		self->y = self->ylen - 1;
		memmove(self->fb, self->fb + self->xlen, self->xlen*(self->ylen - 1)*2);
		memset(self->fb + self->xlen*(self->ylen - 1), 0, self->xlen*2);
	}
}
void text_advance_col(Text *self) {
	self->x++;
	if (self->x >= self->xlen) {
		self->x = 0;
		text_advance_row(self);
	}
}
void text_putchar(Text *self, char c) {
	self->fb[self->xlen*self->y + self->x] = c | self->attr << 8;
}

ssize_t text_write(IoFile *file, void *buf, size_t n, off_t off) {
	u8 *src = buf;
	Text *text = file->inner;

	for (size_t i = 0; i < n; i++) {
		switch (src[i]) {
		case '\r':
			text->x = 0;
			break;
		case '\n':
			text->x = 0;
			text_advance_row(text);
			break;
		case '\t':
			for (int i = 0; i < 8; i++)
				text_advance_col(text);
			break;
		default:
			text_putchar(text, src[i]);
			text_advance_col(text);
		}
	}

	return n;
}