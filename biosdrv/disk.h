#ifndef BIOSDRV_DISK_H
#define BIOSDRV_DISK_H

#include <io/file.h>

ssize_t biosdrv_disk_read(IoFile *file, void *buf, size_t n, off_t off);

#endif