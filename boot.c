#include <mem/mem.h>
#include <bios/info.h>
#include <bios/call.h>
#include <bios/linker.h>
#include <str/str.h>
#include <man.h>

int main() {
	short *fb = (short*)0xb8000;

	for (int i = 0; i < 80*25; i++)
		fb[i] = 0;

	memmove(fb, &main, 80*25*2);

	return 0;
}