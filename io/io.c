#include "io.h"

#include <stddef.h>

ssize_t io_write(IoFile *file, void *buf, size_t n) {
	if (file->write == nil)
		return 0;

	ssize_t ret = file->write(file, buf, n, file->pos);
	if (ret >= 0)
		file->pos += ret;
	return ret;
}

ssize_t io_read(IoFile *file, void *buf, size_t n) {
	if (file->read == nil)
		return 0;

	ssize_t ret = file->read(file, buf, n, file->pos);
	if (ret >= 0)
		file->pos += ret;
	return ret;
}

ssize_t io_pwrite(IoFile *file, void *buf, size_t n, off_t off) {
	if (file->write == nil)
		return 0;

	return file->write(file, buf, n, off);
}

ssize_t io_pread(IoFile *file, void *buf, size_t n, off_t off) {
	if (file->read == nil)
		return 0;

	return file->read(file, buf, n, off);
}