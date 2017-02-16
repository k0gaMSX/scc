/* See LICENSE file for copyright and license details. */

#include <string.h>

char *
strncpy(char *dst, const char *src, size_t n)
{
	char *ret = dst;

	for (; n > 0 && *src; --n)
		*dst++ = *src++;
	while (n-- > 0)
		*dst++ = '\0';
	return ret;
}
