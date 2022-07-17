#ifndef ELF_ERRORS_H
#define ELF_ERRORS_H

enum elf_error {
	ELF_OK = 0,

	// Signifies an I/O function error
	ELF_IO_ERROR,

	// Returned by elf_init when the file
	// specified phentsize is not sizeof(ElfProgram)
	ELF_INVALID_PHENTSIZE,

	// End of program entry array
	ELF_EOP,

	// Returned by elf_init when the file does
	// not contain a valid magic
	ELF_NO_MAGIC
};

#endif