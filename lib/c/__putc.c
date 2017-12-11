
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include "syscall.h"

int
_fflush(FILE *fp)
{
	int lnbuf = fp->flags & _IOLBF;
	size_t cnt;

	cnt = ((lnbuf) ? fp->lp : fp->wp) - fp->buf;

	if (_write(fp->fd, fp->buf, cnt) != cnt) {
		fp->flags |= _IOERR;
		return EOF;
	}
	fp->rp = fp->wp = fp->buf;

	return 0;
}

int
fflush(FILE *fp)
{
	int err = 0;

	if (fp)
		return _flsbuf(fp);

	for (fp = __iob; fp < &__iob[FOPEN_MAX]; ++fp) {
		if ((fp->flags & _IOWRITE) == 0 && _flush(fp))
			err = EOF;
	}
	return err;
}

static void
cleanup(void)
{
	fflush(NULL);
}

int
__putc(int ch, FILE *fp)
{
	static int first = 1;

	if (fp->flags & _IOERR)
		return EOF;

	if (fp->flags & _IOREAD) {
		fp->flags |= _IOERR;
		errno = EBADF;
		return EOF;
	}

	if (fp->flags & _IOSTRG) {
		fp->flags |= _IOERR;
		return EOF;
	}

	if (first) {
		if (atexit(cleanup)) {
			fp->flags |= _IOERR;
			errno = ENOMEM;
			return EOF;
		}
		first = 0;
	}

	if (fp->flags & _IOLBF) {
		if (fp->lp == fp->rp && _flush(fp))
			return EOF;
		*fp->lp++ = ch;
		if (ch == '\n' && flsbuf(fp))
			return EOF;
	} else {
		if (fp->wp == fp->rp && _flsbuf(fp))
			return EOF;
		*fp->wp++ = ch;
	}

done:
	fp->flags |= _IOWRITE;
	return ch & 0xFF;
}
