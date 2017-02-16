/* See LICENSE file for copyright and license details. */

#include <string.h>

char *
strcat(char *dst, const char *src)
{
	char *ret;

	for (ret = dst; *dst; ++dst)
		/* nothing */;
	while (*dst++ = *src++)
		/* nothing */;
	return ret;
}
