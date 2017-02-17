/* See LICENSE file for copyright and license details. */

#include <string.h>

int
strcoll(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 != *s2)
		++s1, ++s2;
	return *s1 - *s2;
}
