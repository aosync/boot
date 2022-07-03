#include "file.h"

#include <io/io.h>

void part_file_init(PartFile *self, IoFile *parent, u64 begin, u64 end) {
	*self = (PartFile) {
		.file = {
			.inner = &self->inner,
			.read = part_file_read
		},
		.inner = {
			.parent = parent,
			.begin = begin,
			.end = end
		}
	};
}

ssize_t part_file_read(IoFile *self, void *buf, size_t n, off_t off) {
	PartFileInner *inner = (PartFileInner*)self->inner;

	if (inner->begin + off + n >= inner->end)
		return -1;
	
	return io_pread(inner->parent, buf, n, inner->begin + off);
}