/* See LICENSE file for copyright and license details. */

#include <stdlib.h>
#undef exit

void (*_atexitf[_ATEXIT_MAX])(void);

void
exit(int status)
{
	void (**bp)(void);
	int i;

	for (i = _ATEXIT_MAX-1; i >= 0; --i) {
		if (bp = _atexit[i]) {
			*_atexit[i] = NULL;
			(*bp)();
		}
	}
	_Exit(status);
}
