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

	// Returns by bs_init when the
	// device has a blksz different than 12
	BS_UNSUPPORTED_BLKSZ,
};

#endif