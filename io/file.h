#ifndef IO_FILE_H
#define IO_FILE_H

#include <stdint.h>

struct io_file;
typedef struct io_file IoFile;

typedef ssize_t (*IoWriter)(IoFile*, void*, size_t, off_t);
typedef ssize_t (*IoReader)(IoFile*, void*, size_t, off_t);
typedef int (*IoWalker)(IoFile*, IoFile*, char*);

struct io_file {
	void *inner;
	IoWriter write;
	IoReader read;
	IoWalker walk;
	off_t pos;
};
typedef struct io_file IoFile;

#endif