#ifndef BS_BS_H
#define BS_BS_H

#include "map.h"

#include <assert.h>
#include <stdint.h>
#include <io/io.h>
#include <mem/framer.h>

struct bs_bootsec {
	u8 bootstrap[446];

	// bsfs signature, 0xCAAA
	u16 signature;

	u8 pad0[40];

	// Inid (inode id) of the root dir
	u64 rootin;

	// Log2 of the block size, only 12 is supported
	u8 blksz;

	u8 pad1[13];

	// MBR magic, 0xAA55
	u16 magic;
}__attribute__((packed));
typedef struct bs_bootsec BsBootsec;
static_assert(sizeof(BsBootsec) == 512, "BsBootsec is 512 bytes");

struct bs_inode {
	// Block id of the inode contents map root 
	u64 blk;

	// Size of the file
	u64 size;

	// Last time the file was modified
	u64 modif;

	// Name of the owner
	u8 owner[17];

	// Name of the group of the owner
	u8 group[17];

	// Refcount of the inode
	u16 refs;

	// Permissions of the file
	u8 perms;

	// File attributes
	u8 attr;

	// Map root level
	u8 level;

	u8 rsvd;
}__attribute__((packed));
typedef struct bs_inode BsInode;
static_assert(sizeof(BsInode) == 64, "BsInode is 64 bytes");

struct bs_bs {
	IoFile *file;
	u64 rootin;
	u8 blksz;
	void *ws;
};
typedef struct bs_bs Bs;

struct bs_file {
	IoFile file;
	Bs *bs;
	BsInode inode;
	BsMap map;
};
typedef struct bs_file BsFile;

struct bs_dirhead {
	u64 next;
	u64 pad0[3];
}__attribute__((packed));
typedef struct bs_dirhead BsDirhead;
static_assert(sizeof(BsDirhead) == 32, "BsDirhead is 32 bytes");

struct bs_dirent {
	char name[24];
	u64 inid;
}__attribute__((packed));
typedef struct bs_dirent BsDirent;
static_assert(sizeof(BsDirent) == 32, "BsDirent is 32 bytes");

int bs_init(Bs *self, IoFile *file);
int bs_inode_fetch(Bs *self, BsInode *inode, u64 inid);

int bs_get_file(Bs *self, BsFile *dst, u64 inid);
int bs_root(Bs *self, BsFile *dst);

ssize_t bs_file_read(IoFile *file, void *buf, size_t n, off_t off);
int bs_file_walk(IoFile *file, IoFile *dst, char *name);

#endif