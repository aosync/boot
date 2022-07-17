#include "io.h"

#include <stddef.h>
#include <str/str.h>

IoFile *io_stdout = nil;

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

int io_walk(IoFile *file, IoFile *dst, char *name) {
	if (file->walk == nil)
		return 1;
	
	return file->walk(file, dst, name);
}

int io_ballad(IoFile *file, char *name) {
	char *tok;
	int iter = 0;
	int rc = 0;

	while ((tok = str_split(name, '/', &iter))) {
		if (!tok[0])
			continue;

		if (io_walk(file, file, tok)) {
			rc = 1;
			break;
		}
	}

	while (str_split(name, '/', &iter));

	return rc;
}