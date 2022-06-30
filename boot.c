#include <mem/mem.h>
#include <bios/info.h>
#include <bios/call.h>
#include <bios/linker.h>
#include <str/str.h>
#include <str/conv.h>
#include <man.h>

#include <mem/framer.h>

int main() {
	short *fb = (short*)0xb8000;

	for (int i = 0; i < 80*25; i++)
		fb[i] = 0;

	char tmp[100];
	str_u64todec(tmp, 4269);
	for (int i = 0; tmp[i]; i++)
		fb[i] = tmp[i] | 0xA << 8;

	return 0;
}