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

extern unsigned char __ctype[];
extern int __ctmp;

#define isalnum(c)  ((__ctype+1)[c] & (_U|_L|_D))
#define isalpha(c)  ((__ctype+1)[c] & (_U|_L))
#define iscntrl(c)  ((__ctype+1)[c] & (_C))
#define isdigit(c)  ((__ctype+1)[c] & (_D))
#define isgraph(c)  ((__ctype+1)[c] & (_P|_U|_L|_D))
#define islower(c)  ((__ctype+1)[c] & (_L))
#define isprint(c)  ((__ctype+1)[c] & (_P|_U|_L|_D|_SP))
#define ispunct(c)  ((__ctype+1)[c] & (_P))
#define isspace(c)  ((__ctype+1)[c] & (_S))
#define isupper(c)  ((__ctype+1)[c] & (_U))
#define isxdigit(c) ((__ctype+1)[c] & (_D|_X))

#define tolower(c) ((__ctmp=c, isupper(__ctmp) ? __ctmp | 0x20 : __ctmp))
#define toupper(c) ((__ctmp=c, islower(__ctmp) ? __ctmp & ~0x20 : __ctmp))

#define isascii(c) ((unsigned)(c)<=0x7f)

#endif

#endif
