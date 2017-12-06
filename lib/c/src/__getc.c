
#include <errno.h>
#include <stdio.h>
#include "syscall.h"
#undef getc

int
__getc(FILE *fp)
{
	int cnt;

	if (fp->flags & (_IOEOF | _IOERR))
		return EOF;

	if ((fp->flags & (_IOREAD | _IORW)) == 0) {
		fp->flags |= _IOERR;
		errno = EBADF;
		return EOF;
	}

	if (fp->flags & _IOSTRG) {
		fp->flags |= _IOEOF;
		return EOF;
	}

	if ((cnt = _read(fp->fd, fp->buf, fp->len)) <= 0) {
		fp->flags |= (cnt == 0) ? _IOEOF : _IOERR;
		return EOF;
	}

	fp->flags |= _IOREAD;
	fp->rp = fp->buf;
	fp->wp = fp->buf + cnt;

	return *fp->rp++;
}
