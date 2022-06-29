#include <mem/mem.h>
#include <bios/info.h>
#include <bios/call.h>
#include <bios/linker.h>
#include <str/str.h>

int main() {
	short *fb = (short*)0xb8000;

	for (int i = 0; i < 80*25; i++)
		fb[i] = 0;


	char *s = "hello";
	int len = strlen(s);
	for (int i = 0; i < len; i++)
		fb[i] = s[i] | 0xA << 8;

	return 0;
}