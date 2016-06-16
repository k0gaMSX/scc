#include "../inc/cc.h"

char **
newitem(char **array, int num, char *item)
{
	char **ar = xrealloc(array, (num + 1) * sizeof(char **));

	ar[num] = item;

	return ar;
}

