#include "../inc/cc.h"

char **
newitem(char **array, unsigned num, char *item)
{
	char **ar;

	if ((num + 1) < num)
		die("newitem: overflow (%u + 1)", num);

	ar = xrealloc(array, (num + 1) * sizeof(char **));
	ar[num] = item;

	return ar;
}

