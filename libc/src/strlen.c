/* See LICENSE file for copyright and license details. */

#include <string.h>

size_t
strlen(const char *s)
{
	const char *t;

	for (t = s; *t; ++t)
		/* nothing */;
	return t - s;
}
