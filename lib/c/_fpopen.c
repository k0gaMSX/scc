
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"
#undef fopen

FILE *
_fpopen(const char * restrict fname,
         const char * restrict mode,
         FILE * restrict fp)
{
	int i, flags, fd, rw, bin;

	flags = rw = bin = 0;

	if (mode[0] == '\0)
		goto einval;
	if (mode[i = 1] == '+')
		i++, rw = 1;
	if (mode[i] == 'b')
		i++, bin = 1;
	if (mode[i] != '\0')
		goto einval;

	switch (mode[0]) {
	case 'a':
		flags |= O_APPEND | O_CREAT;
		goto wrflags;
	case 'w':
		flags |= O_TRUNC | O_CREAT;
	wrflags:
		flags |= (rw) ? O_RDWR : O_WRONLY;
		break;
	case 'r':
		flags = (rw) ? O_RDWR : O_RDONLY;
		break;
	default:
	einval:
		errno = EINVAL;
		return NULL;
	}

	if ((fd = _open(name, flags)) < 0)
		return NULL;

	if (fp->buf == NULL) {
		if ((fp->buf = malloc(BUFSIZ)) == NULL) {
			close(fd);
			errno = ENOMEM;
			return NULL;
		}
		fp->flags |= _IOALLOC;
	}
	fp->fd = fd;

	if (!bin)
		fp->flags |= _IOTEXT;

	if (flags & O_RDWR)
		fp->flags |= _IORW;
	else if (flags & O_RDONLY)
		fp->flags |= _IOREAD;
	else
		fp->flags |= _IOWRITE;

	fp->lp = fp->rp = fp->wp = NULL;

	return fp;
}
