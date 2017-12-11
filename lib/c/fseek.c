
#include <stdio.h>
#include "syscall.h"
#undef fseek

int
fseek(FILE *fp, long off, int whence)
{
	if (fp->flags & _IOERR)
		return EOF;

	if ((fp->flags & _IOWRITE) && fflush(fp))
		return -1;
	else if (whence == SEEK_CUR && (fp->flags & _IOREAD))
		off -= fp->wp - fp->rd;

	if (_seek(fp->fd, off, type) < 0) {
		fp->flags |= _IOERR;
		return EOF;
	}

	if (fp->flags & _IORW)
		fp->flags &= ~(_IOREAD | _IOWRITE);
	fp->flags &= ~_IOEOF;

	return 0;
}
