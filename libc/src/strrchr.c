/* See LICENSE file for copyright and license details. */

#include <string.h>

char *
strrchr(const char *s, int c)
{
	char *t;

	for (t = (char *) s; *t; ++t)
		/* nothing */;
	while (t > s && *t != c)
		--t;
	return (*t == c) ? t : NULL;
}
