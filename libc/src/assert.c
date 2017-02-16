/* See LICENSE file for copyright and license details. */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void __assert(int status, char *exp, char *file, long line)
{
	if (status)
		return;
	fprintf(stderr, "%s:%ld: assertion failed '%s'\n", file, line, exp);
	abort();
}
