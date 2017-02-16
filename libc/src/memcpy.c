/* See LICENSE file for copyright and license details. */

#include <string.h>

void *
memcpy(void *dst, const void *src, size_t n)
{
	char *s1 = dst, *s2 = (char *) src;

	while (n-- > 0)
		*s1++ = *s2++;
	return dst;
}
