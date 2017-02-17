/* See LICENSE file for copyright and license details. */

#define __USE_MACROS
#include <ctype.h>
#undef isspace

int
isspace(int c)
{
	return _ctype[(unsigned char) c] & _S;
}
