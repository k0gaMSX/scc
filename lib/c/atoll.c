#include <ctype.h>
#include <stdlib.h>
#undef atoll

long long
atoll(const char *s)
{
	int sign = -1;
	long long n;

	while (isspace(*s))
		++s;

	switch (*s) {
	case '-':
		sign = 1;
	case '+':
		++s;
	}

	/* Compute n as a negative number to avoid overflow on LLONG_MIN */
	for (n = 0; isdigit(*s); ++s)
		n = 10*n - (*s - '0');

	return sign * n;
}

