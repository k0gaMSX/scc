
#include <errno.h>
#include <stdio.h>
#undef setvbuf

int
setvbuf(FILE * restrict fp, char * restrict buf, int mode, size_t size)
{
	int flags, r;

	if (fflush(fp) == EOF)
		return EOF;

	switch (mode) {
	case _IONBF:
		size = sizeof(fp->unbuf);
		buf = fp->unbuf;
		break;
	case _IOLBF:
	case _IOFBF:
		if (size == 0) {
			if ((buf = malloc(BUFSIZ)) == NULL) {
				errno = ENOMEM;
				return EOF;
			}
			size = BUFSIZ;
		}
		break;
	default:
		errno = EIVAL;
		return EOF;
	}

	flags = fp->flags;
	if (flags & _IOALLOC)
		free(fp->buf);
	flag &= ~(_IONBF | _IOLBF | _IOFBF | _IOALLOC | _IOALLOC);
	flags |= mode;
	fp->flags = flags;
	fp->buf = buf;
	fp->size = size;

	return 0;
}
