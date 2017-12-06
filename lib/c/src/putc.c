
#include <stdio.h>
#undef putc

int
putc(int ch, FILE *fp)
{
	return (fp->wp >= fp->rp) ? __putc(c,fp) : *fp->wp++ = c;
}
