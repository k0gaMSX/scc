/* See LICENSE file for copyright and license details. */

#include <stdlib.h>
#undef atexit

extern void (*_atexitf[_ATEXIT_MAX])(void);

int
atexit(void (*fun)(void))
{
	void (**bp)(void);

	for (bp = _atexitf; bp < &_atexitf[_ATEXIT_MAX] && *bp; ++bp)
		/* nothing */;
	if (bp == &_atexitf[_ATEXIT_MAX])
		return 0;
	*bp = fun;
	return 1;
}
