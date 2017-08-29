#define __USE_MACROS
#include <ctype.h>
#undef iscntrl

int
iscntrl(int c)
{
	return __ctype[(unsigned char) c] & (_C);
}
