/* See LICENSE file for copyright and license details. */

#include <string.h>

char *
strcat(char *dst, const char *src)
{
	char *ret = dst;

	while (*dst)
		++dst;
	while (*dst++ = *src++)
		/* nothing */;
	return ret;
}
