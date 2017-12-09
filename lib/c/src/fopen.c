
#include <errno.h>
#include <stdio.h>
#undef fopen

FILE *
fopen(const char * restrict name, const char * restrict mode)
{
	FILE *fp;

	for (fp = __iob; fp < &__iob[FILE_MAX]; ++fp) {
		if (fp->flags & (_IOREAD | _IOWRITE | _IORW) == 0)
			break;
	}
	if (fp == &__iob[FILE_MAX]) {
		errno = ENOMEM;
		return NULL;
	}
	return _fpopen(name, mode, fp);
}
