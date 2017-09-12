#define __USE_MACROS
#include <ctype.h>
#undef isspace

int
isspace(int c)
{
	return (__ctype+1)[c] & _S;
}
