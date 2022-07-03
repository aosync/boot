#include "part.h"

#include <fmt/fmt.h>
#include <part/mbr.h>

int find_boot_part(PartFile *file) {
	PartMbr mbr;
	ssize_t ret = io_read(sys->disk, &mbr, 512);
	if (ret != 512) {
		printf("No MBR.\n");
		return 0;
	}

	for (int i = 0; i < 4; i++)
		if (mbr.entries[i].attributes & 1) {
			part_file_init(file, sys->disk, mbr.entries[i].lbegin, mbr.entries[i].lbegin + mbr.entries[i].length);
			return 1;
		}

	return 0;
}