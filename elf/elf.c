#include "elf.h"
#include "errors.h"

int elf_init(Elf *elf, IoFile *file) {
	*elf = (Elf) {
		.file = file
	};

	if (io_pread(file, &elf->header, sizeof(ElfHeader), 0) != sizeof(ElfHeader))
		return ELF_IO_ERROR;

	if (elf->header.magic != ELF_MAGIC)
		return ELF_NO_MAGIC;
	
	if (elf->header.phentsize != sizeof(ElfPhent))
		return ELF_INVALID_PHENTSIZE;
	
	return ELF_OK;
}

int elf_phent_get(Elf *elf, ElfPhent *dst, size_t idx) {
	if (idx >= elf->header.phnum)
		return ELF_EOP;
	
	if (io_pread(elf->file, dst, sizeof(ElfPhent), elf->header.phoff + idx * sizeof(ElfPhent)) != sizeof(ElfPhent))
		return ELF_IO_ERROR;
	
	return ELF_OK;
}