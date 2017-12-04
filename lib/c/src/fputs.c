
#include <stdio.h>

int
fputs(const char * restrict bp, FILE * restrict fp)
{
	int r, ch;

	while (ch = *bp++)
		r = putc(ch, fp);
	return r;
}
