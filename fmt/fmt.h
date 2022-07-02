#ifndef FMT_FMT_H
#define FMT_FMT_H

#include <stdint.h>
#include <stdarg.h>
#include <io/file.h>

int _fprintf(IoFile *file, char *fmt, va_list list);
int fprintf(IoFile *file, char *fmt, ...);
int printf(char *fmt, ...);

#endif