/* See LICENSE file for copyright and license details. */
#ifndef _CTYPE_H
#define _CTYPE_H

extern int isalnum(int c);
extern int isalpha(int c);
extern int islower(int c);
extern int isupper(int c);
extern int isdigit(int c);
extern int isxdigit(int c);
extern int iscntrl(int c);
extern int isgraph(int c);
extern int isspace(int c);
extern int isblank(int c);
extern int isprint(int c);
extern int ispunct(int c);
extern int tolower(int c);
extern int toupper(int c);

#ifdef __USE_MACROS

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex char */
#define _SP	0x80	/* hard space (0x20) */
#define _TB	0x100	/* tabulation */

extern unsigned char _ctype[];

#define isalnum(c)  (_ctype[(unsigned char) c] & (_U|_L|_D))
#define isalpha(c)  (_ctype[(unsigned char) c] & (_U|_L))
#define isblank(c)  (_ctype[(unsigned char) c] & (_SP|_TB))
#define iscntrl(c)  (_ctype[(unsigned char) c] & (_C))
#define isdigit(c)  (_ctype[(unsigned char) c] & (_D))
#define isgraph(c)  (_ctype[(unsigned char) c] & (_P|_U|_L|_D))
#define islower(c)  (_ctype[(unsigned char) c] & (_L))
#define isprint(c)  (_ctype[(unsigned char) c] & (_P|_U|_L|_D|_SP))
#define ispunct(c)  (_ctype[(unsigned char) c] & (_P))
#define isspace(c)  (_ctype[(unsigned char) c] & (_S))
#define isupper(c)  (_ctype[(unsigned char) c] & (_U))
#define isxdigit(c) (_ctype[(unsigned char) c] & (_D|_X))

#define isascii(c) (((unsigned) c)<=0x7f)

#endif

#endif
