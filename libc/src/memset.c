/* See LICENSE file for copyright and license details. */

#include <string.h>

void *
memset(void *s, int c, size_t n)
{
	char *bp;

	for (bp = s; n-- > 0; *bp++ = c)
		/* nothing */;
	return s;
}
