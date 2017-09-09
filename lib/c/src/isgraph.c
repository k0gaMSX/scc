#define __USE_MACROS
#include <ctype.h>
#undef isgraph

int
isgraph(int c)
{
	return __ctype[(unsigned char) c] & (_P|_U|_L|_D);
}