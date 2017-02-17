#include <ctype.h>

#ifdef __USE_MACROS

unsigned char _ctype[255] = {
	_C,_C,_C,_C,_C,_C,_C,_C,                        /* 0-7 */
	_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,         /* 8-15 */
	_C,_C,_C,_C,_C,_C,_C,_C,                        /* 16-23 */
	_C,_C,_C,_C,_C,_C,_C,_C,                        /* 24-31 */
	_S|_SP,_P,_P,_P,_P,_P,_P,_P,                    /* 32-39 */
	_P,_P,_P,_P,_P,_P,_P,_P,                        /* 40-47 */
	_D,_D,_D,_D,_D,_D,_D,_D,                        /* 48-55 */
	_D,_D,_P,_P,_P,_P,_P,_P,                        /* 56-63 */
	_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,      /* 64-71 */
	_U,_U,_U,_U,_U,_U,_U,_U,                        /* 72-79 */
	_U,_U,_U,_U,_U,_U,_U,_U,                        /* 80-87 */
	_U,_U,_U,_P,_P,_P,_P,_P,                        /* 88-95 */
	_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,      /* 96-103 */
	_L,_L,_L,_L,_L,_L,_L,_L,                        /* 104-111 */
	_L,_L,_L,_L,_L,_L,_L,_L,                        /* 112-119 */
	_L,_L,_L,_P,_P,_P,_P,_C,                        /* 120-127 */
};

#else

int
isblank(int c)
{
	return (c == ' ') || (c == '\t');
}

int
iscntrl(int c)
{
	return (unsigned)c < ' ' || c == 0x7f;
}

int
isdigit(int c)
{
	return (unsigned)c - '0' < 10;
}

int
islower(int c)
{
	return (unsigned)c - 'a' < 26;
}

int
isupper(int c)
{
	return (unsigned)c - 'A' < 26;
}

int
isprint(int c)
{
	return (unsigned)c - ' ' < 95;
}

int
isspace(int c)
{
	return (c == ' ') || ((unsigned)c - '\t' < 5);
}

int
isxdigit(int c)
{
	return isdigit(c) ||
	       ((unsigned)c - 'a' < 6) ||
	       ((unsigned)c - 'A' < 6);
}

int
isgraph(int c)
{
	return isprint(c) && c != ' ';
}

int
isalpha(int c)
{
	return islower(c) || isupper(c);
}

int
isalnum(int c)
{
	return isalpha(c) || isdigit(c);
}

int
ispunct(int c)
{
	return isprint(c) && !isspace(c) && !isalnum(c);
}

int
tolower(int c)
{
	return (isupper(c) ? (unsigned)c + ' ' : c);
}

int
toupper(int c)
{
	return (islower(c) ? (unsigned)c - ' ' : c);
}

#endif // __USE_MACROS
