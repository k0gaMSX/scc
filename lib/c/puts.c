
#include <stdio.h>

int
puts(const char *str)
{
	int ch;

	while (ch = *str)
		putchar(ch);
	return putchar('\n');
}
