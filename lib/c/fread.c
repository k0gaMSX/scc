
#include <stdio.h>
#undef fread

size_t
fread(void * restrict ptr, size_t size, size_t nmemb,
      FILE * restrict fp)
{
	unsigned char *bp = ptr;
	size_t n, i;

	if (size == 0)
		return 0;

	for (n = 0; n < nmemb; n++) {
		i = size;
		do {
			if ((*bp++ = getc(fp)) == EOF)
				return n;
		} while (--i);
	}

	return n;
}
