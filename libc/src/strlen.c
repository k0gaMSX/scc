/* See LICENSE file for copyright and license details. */

#include <string.h>

size_t
strlen(const char *s)
{
	size_t n = 0;

	while (*s++)
		++n;
	return n;
}
