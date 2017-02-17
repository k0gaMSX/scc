/* See LICENSE file for copyright and license details. */

#define __USE_MACROS
#include <ctype.h>
#undef isdigit

int
isdigit(int c)
{
	return _ctype[(unsigned char) c] & (_D);
}
