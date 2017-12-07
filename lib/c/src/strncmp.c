#include <string.h>
#undef strncmp

int
strncmp(const char *s1, const char *s2, size_t n)
{
	int c;

	if (n == 0)
		return 0;
	while ((c = *s1) != '\0' && c == *s2) {
		if (--n == 0)
			return 0;
		++s1, ++s2;
	}
	return (*(unsigned char *) s1 - *(unsigned char *) s2);
}
