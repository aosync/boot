#ifndef BS_ERRORS_H
#define BS_ERRORS_H

enum bs_errors {
	BS_OK = 0,

	// Returned by bs_init when the
	// device does not have the MBR magic 0xAA55
	BS_NO_MAGIC,

	// Returned by bs_init when the
	// device does not have the bsfs signature 0xCAAA
	BS_NO_SIGNATURE,

	// Returned by bs_init when the
	// device has a blksz different than 12
	BS_UNSUPPORTED_BLKSZ,

	// Returned when an invalid argument was provided
	BS_INVAL,

	// Returned by bs_get_file when the inid does
	// not correspond to an allocated inode
	BS_FREE_INODE,

	// The filesystem does not have a root directory
	BS_NO_ROOT,

	// Returned when an I/O operation failed
	BS_IO_ERROR
};

#endif