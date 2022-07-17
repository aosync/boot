#ifndef ELF_ELF_H
#define ELF_ELF_H

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#include <io/io.h>

#define ELF_MAGIC (0x7F | 0x45 << 8 | 0x4c << 16 | 0x46 << 24)

enum elf_class {
	ELF_CLASS_32 = 1,
	ELF_CLASS_64 = 2,
};

enum elf_abi {
	ELF_ABI_SYSV = 0x0,
	ELF_ABI_NETBSD = 0x2,
	ELF_ABI_LINUX = 0x3,
	ELF_ABI_SOLARIS = 0x6,
	ELF_ABI_FREEBSD = 0x9,
	ELF_ABI_OPENBSD = 0xC,
};

enum elf_type {
	ELF_TYPE_NONE = 0x0,
	ELF_TYPE_REL = 0x1,
	ELF_TYPE_EXEC = 0x2,
	ELF_TYPE_DYN = 0x3,
	ELF_TYPE_CORE = 0x4
};

enum elf_machine {
	ELF_MACHINE_X86 = 0x3,
	ELF_MACHINE_ARM = 0x28
};

// NOTE: this is ELF64 only
struct elf_header {
	u32 magic;
	u8 class;
	u8 endianness;
	u8 version;
	u8 osabi;
	u8 abiversion;
	u8 pad0[7];
	u16 type;
	u16 machine;
	u32 version2;
	u64 entry;
	u64 phoff;
	u64 shoff;
	u32 flags;
	u16 ehsize;
	u16 phentsize;
	u16 phnum;
	u16 shentsize;
	u16 shnum;
	u16 shstrndx;
}__attribute__((packed));
typedef struct elf_header ElfHeader;
static_assert(sizeof(ElfHeader) == 64, "ElfHeader is 64 bytes");

enum elf_phent_type {
	ELF_PHENT_NULL = 0x0,
	ELF_PHENT_LOAD = 0x1,
	ELF_PHENT_DYNAMIC = 0x2,
	ELF_PHENT_INTERP = 0x3,
	ELF_PHENT_NOTE = 0x4,
	ELF_PHENT_SHLIB = 0x5,
	ELF_PHENT_PHDR = 0x6,
	ELF_PHENT_TLS = 0x7
};

struct elf_phent {
	u32 type;
	u32 flags;
	u64 offset;
	u64 vaddr;
	u64 paddr;
	u64 filesz;
	u64 memsz;
	u64 align;
}__attribute__((packed));
typedef struct elf_phent ElfPhent;
static_assert(sizeof(ElfPhent) == 56, "ElfPhent is 56 bytes");

struct elf_elf {
	IoFile *file;
	ElfHeader header;
};
typedef struct elf_elf Elf;

int elf_init(Elf *elf, IoFile *file);
int elf_phent_get(Elf *elf, ElfPhent *dst, size_t idx);

#endif