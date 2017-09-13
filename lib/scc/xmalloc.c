static char sccsid[] = "@(#) ./lib/scc/xmalloc.c";
#include <stdlib.h>
#include "../../inc/scc.h"

void *
xmalloc(size_t size)
{
	void *p = malloc(size);

	if (!p)
		die("out of memory");
	return p;
}
