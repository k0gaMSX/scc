#include <stdio.h>
#undef fgets

char *
fgets(char *s, int n, FILE *fp)
{
	int ch;
	char *t = s;

	while (--n > 0 && (ch = getc(fp)) != EOF) {
		if ((*t++ = ch) == '\n')
			break;
	}
	if (ch == EOF && s == t)
		return NULL;
	*t = '\0';

	return s;
}
