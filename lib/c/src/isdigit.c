#define __USE_MACROS
#include <ctype.h>
#undef isdigit

int
isdigit(int c)
{
	return __ctype[(unsigned char) c] & (_D);
}
