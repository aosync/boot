#ifndef BIOSDRV_DISK_H
#define BIOSDRV_DISK_H

#include <io/file.h>

#define BIOSDRV_DISK_ID 0xF9A1992769935754

char biosdrv_disk_init(void);
ssize_t biosdrv_disk_read(IoFile *file, void *buf, size_t n, off_t off);

#endif