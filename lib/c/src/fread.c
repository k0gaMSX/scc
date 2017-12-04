
#include <stdio.h>
#undef fread

size_t
fread(void * restrict ptr, size_t size, size_t nmemb,
      FILE * restrict fp)
{
	unsigned char *bp = ptr;
	size_t n, i;

	if (nmemb == 0 || size == 0)
		return 0;

	for (n = 0; n < nmemb; n++) {
		for (i = 0; i < size; ++i) {
			if ((*bp++ = getc(fp)) == EOF)
				return n;
		}
	}

	return n;
}
