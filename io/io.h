#ifndef IO_IO_H
#define IO_IO_H

#include "file.h"

#include <stdint.h>

ssize_t io_pwrite(IoFile *file, void *buf, size_t n, off_t off);
ssize_t io_pread(IoFile *file, void *buf, size_t n, off_t off);

ssize_t io_write(IoFile *file, void *buf, size_t n);
ssize_t io_read(IoFile *file, void *buf, size_t n);

#endif