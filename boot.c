#include <mem/mem.h>
#include <bios/info.h>
#include <bios/call.h>
#include <bios/linker.h>

int main() {
	short *fb = (short*)0xb8000;

	for (int i = 0; i < 80*25; i++)
		fb[i] = 0;

	return 0;
}