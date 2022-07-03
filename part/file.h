#ifndef PART_FILE_H
#define PART_FILE_H

#include <stdint.h>
#include <io/file.h>

struct part_file_inner {
	IoFile *parent;
	u64 begin;
	u64 end;
};
typedef struct part_file_inner PartFileInner;

struct part_file {
	IoFile file;
	PartFileInner inner;
};
typedef struct part_file PartFile;

void part_file_init(PartFile *self, IoFile *parent, u64 begin, u64 end);
ssize_t part_file_read(IoFile *self, void *buf, size_t n, off_t off);

#endif