#ifndef IO_ERRORS_H
#define IO_ERRORS_H

enum io_error {
	IO_OK = 0,

	// File read is unexpectedly truncated while reading
	// a structude
	IO_TRUNCATED,

	// File is not mapped at offset
	IO_UNMAPPED,

	// Directory operation was done on a non-directory file
	IO_NOT_DIR,

	// Directory entry was not found
	IO_NOT_FOUND,
	
	// Retrieval of file failed
	IO_RETRIEVAL
};

#endif