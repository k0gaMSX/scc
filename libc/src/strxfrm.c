/* See LICENSE file for copyright and license details. */

#include <string.h>

size_t
strxfrm(char *dst, const char *src, size_t n)
{
	size_t len = strlen(src);

	if (len < n)
		strcpy(dst, src);
	return len;
}
