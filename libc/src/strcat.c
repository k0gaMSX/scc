/* See LICENSE file for copyright and license details. */

#include <string.h>

char *
strcat(char * restrict dst, const char * restrict src)
{
	char *ret = dst;

	while (*dst)
		++dst;
	while (*dst++ = *src++)
		/* nothing */;
	return ret;
}
