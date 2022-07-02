#include <sys/sys.h>
#include <io/io.h>
#include <str/str.h>

#include <mem/mmap.h>
#include <fmt/fmt.h>
#include <text/text.h>

char *mmap_entry_type_name(u32 type) {
	switch (type) {
	case MEM_MMAP_USABLE:
		return "USABLE";
	case MEM_MMAP_RESERVED:
		return "RESERVED";
	case MEM_MMAP_ACPI_RECLAIMABLE:
		return "ACPI_RECLAIMABLE";
	case MEM_MMAP_ACPI_NVS:
		return "ACPI_NVS";
	case MEM_MMAP_BAD:
		return "BAD";
	case MEM_MMAP_BOOTLOADER_RECLAIMABLE:
		return "BOOTLOADER_RECLAIMABLE";
	case MEM_MMAP_KERNEL:
		return "KERNEL";
	case MEM_MMAP_BIOS_IVT:
		return "BIOS_IVT";
	case MEM_MMAP_BIOS_BDA:
		return "BIOS_BDA";
	case MEM_MMAP_BIOS_EBDA:
		return "BIOS_EBDA";
	default:
		return "UNKNOWN";
	}
}

int boot(Sys *sys) {
	MemMmapEntry *entry = (MemMmapEntry*)sys->mmap->first;
	while (entry) {
		printf("%09p %09p %s\n", (u64)entry->begin, (u64)entry->end, mmap_entry_type_name(entry->type));

		entry = (MemMmapEntry*)entry->next;
	}


	return 0;
}