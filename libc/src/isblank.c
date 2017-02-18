/* See LICENSE file for copyright and license details. */

#define __USE_MACROS
#include <ctype.h>
#undef isblank

int
isblank(int c)
{
	return _ctype[(unsigned char) c] & (_SP|_TB);
}
