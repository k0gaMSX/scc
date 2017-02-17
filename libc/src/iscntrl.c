/* See LICENSE file for copyright and license details. */

#define __USE_MACROS
#include <ctype.h>
#undef iscntrl

int
iscntrl(int c)
{
	return _ctype[(unsigned char) c] & (_C);
}
