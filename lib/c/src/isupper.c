#define __USE_MACROS
#include <ctype.h>
#undef isupper

int
isupper(int c)
{
	return __ctype[(unsigned char) c] & _U;
}
