#include <sys/sys.h>
#include <io/io.h>

int boot(Sys *sys) {
	short *fb = (short*)0xB8000;

	u8 *frame = mem_framer_alloc(sys->bulk, 1);
	
	io_read(sys->disk, frame, 512);
	if (frame[510] == 0x55 && frame[511] == 0xAA) {
		fb[0] = 'Y' | 0xB << 8;
	}

	return 0;
}