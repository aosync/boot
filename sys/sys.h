#ifndef SYS_SYS_H
#define SYS_SYS_H

#include <io/file.h>
#include <mem/framer.h>
#include <mem/mmap.h>

// Stores information and methods about the current
// bootloader system.
struct sys_sys {
	MemFramer *prim;
	MemFramer *bulk;

	MemMmapBlk *mmap;

	IoFile *disk;
	IoFile *cons;
};
typedef struct sys_sys Sys;

extern Sys *sys;

#endif