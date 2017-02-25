/* See LICENSE file for copyright and license details. */

#include <ctype.h>
#include <stdlib.h>

int
atoi(const char *s)
{
	int n, sign = 1;

	while(isspace(*s))
		++s;

	switch(*s) {
	case '-':
		sign = -1;
	case '+':
		++s;
	}

	for (n = 0; isdigit(*s); ++s)
		n = 10 * n + (*s - '0');

	return sign * n;
}

