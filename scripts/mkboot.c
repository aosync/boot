#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN64
#include <process.h>
#endif

int main(int argc, char **argv) {
	if (argc < 4)
		return 1;

	char cmd[4097];

	sprintf(cmd, "%s %s format ::%s", argv[1], argv[2], argv[3]);
	system(cmd);

	sprintf(cmd, "%s %s mkdir /boot", argv[1], argv[2]);
	system(cmd);

	for (int i = 4; i < argc; i += 2) {
		sprintf(cmd, "%s %s cp ::%s /boot/%s", argv[1], argv[2], argv[i], argv[i + 1]);
		system(cmd);
	}

	return 0;
}