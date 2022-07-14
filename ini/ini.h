#ifndef INI_INI_H
#define INI_INI_H

#include <io/io.h>

struct ini_ini {
	IoFile *file;
};
typedef struct ini_ini Ini;

void ini_init(Ini *self, IoFile *file);
ssize_t ini_next_until(Ini *self, char *dst, char n);
int ini_get(Ini *self, char *key, char *dst);

#endif