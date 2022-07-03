#include "mbr.h"



char part_mbr_init(PartMbr *self, IoFile *file) {
	ssize_t ret = io_read(file, self, 512);

	if (ret != 512)
		return 0;

	if (self->magic != 0xAA55)
		return 0;
	
	return 1;
}