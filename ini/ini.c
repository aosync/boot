#include "ini.h"
#include "errors.h"

#include <str/str.h>

#include <fmt/fmt.h>

void ini_init(Ini *self, IoFile *file) {
	*self = (Ini) {
		.file = file
	};
}

ssize_t ini_next_until(Ini *self, char *key, char n) {
	ssize_t k = 0;
	ssize_t count;

	char tmp[65];
	while ((count = io_read(self->file, tmp, 64)) > 0) {
		tmp[count] = '\0';

		int i;
		for (i = 0; tmp[i] && tmp[i] != n; i++)
			key[k + i] = tmp[i];

		key[k + i] = '\0';

		k += i;

		if (tmp[i] == n)
			break;
	}

	return k + 1;
}

int ini_get(Ini *self, char *key, char *dst) {
	ssize_t prevpos = self->file->pos;
	self->file->pos = 0;

	char tmp[129];
	ssize_t idx, iidx = 0;

	while ((idx = ini_next_until(self, tmp, '=')) > 0) {
		iidx += idx;
		self->file->pos = iidx;
		if (strcmp(tmp, key) == 0) {
			ini_next_until(self, dst, '\n');
			self->file->pos = prevpos;
			return INI_OK;
		}

		iidx += ini_next_until(self, tmp, '\n');
		self->file->pos = iidx;
	}

	self->file->pos = prevpos;
	return INI_NOT_FOUND;
}