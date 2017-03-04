/* See LICENSE file for copyright and license details. */

#include <stdlib.h>
#undef exit

void (*_atexitf[_ATEXIT_MAX])(void);

void
exit(int status)
{
	void (**bp)(void);

	for (bp = _atexitf; bp < &_atexitf[_ATEXIT_MAX] && *bp; ++bp)
		(*bp)();
	_Exit();
}
