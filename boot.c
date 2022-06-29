#include <mem/test.h>

int main() {

	if (test() == 42) {
		short *fb = (short*)0xb8000;

		for (int i = 0; i < 80*25; i++)
			fb[i] = 0;
	}

	return 0;
}