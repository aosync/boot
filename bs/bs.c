#include "bs.h"
#include "errors.h"

#include <str/str.h>

#define BLKSZ(X) (1 << (X))

// Initializes a bs structure from a file
int bs_init(Bs *self, IoFile *file) {
	BsBootsec bootsec;

	io_pread(file, &bootsec, sizeof(BsBootsec), 0);

	if (bootsec.magic != 0xAA55)
		return BS_NO_MAGIC;

	if (bootsec.signature != 0xCAAA)
		return BS_NO_SIGNATURE;

	if (bootsec.blksz != 12)
		return BS_UNSUPPORTED_BLKSZ;
	
	*self = (Bs) {
		.file = file,
		.rootin = bootsec.rootin,
		.blksz = bootsec.blksz
	};

	return BS_OK;
}

int bs_inode_fetch(Bs *self, BsInode *inode, u64 inid) {
	u64 inaddr = inid << 6;
	return io_pread(self->file, inode, sizeof(BsInode), inaddr) != sizeof(BsInode);
}

int bs_get_file(Bs *self, BsFile *dst, u64 inid) {
	if (inid == 0)
		return 1;
	
	*dst = (BsFile) {
		.file = (IoFile) {
			.inner = &dst->inner,
			.read = bs_file_read,
			.walk = bs_file_walk
		},
		.inner = (BsFileInner) {
			.bs = self
		},
	};

	if (bs_inode_fetch(self, &dst->inner.inode, inid))
		return 1;

	if (dst->inner.inode.rsvd == 0)
		return 2;

	dst->inner.map = (BsMap) {
		.file = self->file,
		.blksz = self->blksz,
		.level = dst->inner.inode.level,
		.base = dst->inner.inode.blk
	};
	bs_map_clear(&dst->inner.map);

	return 0;
}

int bs_root(Bs *self, BsFile *dst) {
	if (self->rootin == 0)
		return 1;
	
	return bs_get_file(self, dst, self->rootin);
}

ssize_t bs_file_read(IoFile *file, void *buf, size_t n, off_t off) {
	BsFileInner *inner = file->inner;
	u16 size = BLKSZ(inner->bs->blksz);

	if (off > inner->inode.size)
		return 0;
	if (off + n > inner->inode.size)
		n = inner->inode.size - off;
	
	u8 *dst = buf;
	u8 *src = inner->bs->ws;

	ssize_t count = 0;

	while (count < n) {
		u64 addr = bs_map_translate(&inner->map, count + off);
		if (addr == 0)
			return -1;
		
		addr -= addr % 4096;

		io_pread(inner->bs->file, src, size, addr);

		size_t head = (count + off) % size;
		while (count < n && head < size) {
			dst[count] = src[head];
			count++;
			head++;
		}
	}

	return count;
}

int bs_file_walk(IoFile *file, IoFile *dst, char *name) {
	BsFileInner *inner = file->inner;

	// Error if not a directory
	if (!(inner->inode.attr & 1))
		return 1;
	
	BsDirhead dhead;
	file->pos = 0;
	if (io_read(file, &dhead, sizeof(BsDirhead)) != sizeof(BsDirhead))
		return 2;

	BsDirent dent;
	for (size_t i = 1; i < dhead.next; i++) {
		if (io_read(file, &dent, sizeof(BsDirent)) != sizeof(BsDirent)) {
			return 3;
		}
		
		if (strcmp(name, dent.name) == 0)
			return bs_get_file(inner->bs, (BsFile*)dst, dent.inid);
	}

	return 4;
}