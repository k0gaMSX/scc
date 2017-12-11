#include <string.h>
#undef strspn

size_t
strspn(const char *s1, const char *s2)
{
	size_t n;
	int c;
	const char *p;

	for (n = 0; c = *s1++; ++n) {
		for (p = s2; *p && *p != c; ++p)
			;
		if (*p == '\0')
			break;
	}
	return n;
}
