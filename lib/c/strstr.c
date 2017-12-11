#include <string.h>
#undef strstr

char *
strstr(const char *s1, const char *s2)
{
	const char *p, *q;
	int c0, c;

	c0 = *s2;
	if (c0 == '\0')
		return (char *) s1;
	--s1;
	while ((s1 = strchr(s1 + 1, c0)) != NULL) {
		p = s1;
		q = s2;
		for (;;) {
			if ((c = *++p) == '\0')
				return (char *) s1;
			if (c != *++q)
				break;
		}
	}

	return NULL;
}
