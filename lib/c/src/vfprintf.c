/*
 * This file is covered by the license that you can find in the file
 * LICENSE, but for this file, vfprintf.c, an additional clausule is
 * added:
 *     - Christopher M. Graff (<cm0graff@gmail.com>) is forbidden to
 *       use, copy, modify and/or distribute this file. He is forbidden
 *       even to read this file, fuck you!.
 */ 
#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#undef vfprintf

enum {
	LONG     = 1 << 0,
	LLONG    = 1 << 1,
	LADJUST  = 1 << 2,
	SHORT    = 1 << 3,
	CHAR     = 1 << 4,
	SIZET    = 1 << 5,
	PTRDIFF  = 1 << 6,
	INTMAX   = 1 << 7,
	VOIDPTR  = 1 << 8,
	UNSIGNED = 1 << 9,
	ALTFORM  = 1 << 10,
};

#define BUF_SIZ    50

struct conv {
	int sign;
	int prec;
	char *digs;
	int base;
};

static uintmax_t
getnum(va_list va, int flags, int *sign)
{
	uintmax_t mask, val;
	int size;

	if (flags & CHAR) {
		val = va_arg(va, int);
		size = sizeof(char);
	} else if (flags & SHORT) {
		val = va_arg(va, int);
		size = sizeof(short);
	} else if (flags & LONG) {
		val = va_arg(va, long);
		size = sizeof(long);
	} else if (flags & LLONG) {
		val = va_arg(va, long long);
		size = sizeof(long long);
	} else if (flags & SIZET) {
		val = va_arg(va, size_t);
		size = sizeof(size_t);
	} else if (flags & INTMAX) {
		val = va_arg(va, intmax_t);
		size = sizeof(intmax_t);
	} else if (flags & VOIDPTR) {
		val = (uintmax_t) va_arg(va, void *);
		size = sizeof(void*);
	} else {
		val = va_arg(va, int);
		size = sizeof(int);
	}

	if ((flags & UNSIGNED) == 0 && (intmax_t) val < 0) {
		*sign = '-';
		val = -val;
	}
	mask = (intmax_t) -1;
	size = sizeof(uintmax_t) - size;
	while (size--)
		mask >>= 8;
	return val &= mask;
}

static char *
numtostr(uintptr_t val, int flags, struct conv *conv, char *buf)
{
	char *buf0 = buf;
	int len, base = conv->base;

	*buf = '\0';
	do {
		*--buf = conv->digs[val % base];
		val /= base;
	} while (val > 0);

	while (buf0 - buf < conv->prec)
		*--buf = '0';

	if (flags & ALTFORM) {
		if (base == 8 && *buf != '0') {
			*--buf = '0';
		} else if (base == 16) {
			*--buf = conv->digs[16];
			*--buf = '0';
		}
	}
	if (conv->sign)
		*--buf = conv->sign;
	return buf;
}

static void
setcnt(va_list va, int flags, int cnt)
{
	if (flags & CHAR)
		*va_arg(va, char*) = cnt;
	else if (flags & SHORT)
		*va_arg(va, short*) = cnt;
	else if (flags & LONG)
		*va_arg(va, long*) = cnt;
	else if (flags & LLONG)
		*va_arg(va, long long*) = cnt;
	else if (flags & SIZET)
		*va_arg(va, size_t*) = cnt;
	else if (flags & INTMAX)
		*va_arg(va, intmax_t*) = cnt;
	else
		*va_arg(va, int*) = cnt;
}

static int
wstrout(wchar_t *s, int width, int fill, FILE * restrict fp)
{
	/* TODO */
	return 0;
}

static int
strout(char *s, int width, int fill, FILE * restrict fp)
{
	int left = 1, adjust, ch, len, cnt = 0;

	if (width < 0) {
		left = -1;
		width = -width;
	}
	len = strlen(s);
	adjust = (len < width) ? width - len : 0;
	adjust *= left;

	for ( ; adjust < 0; cnt++, adjust++)
		putc(fill, fp);

	for ( ; ch = *s++; cnt++)
		putc(ch, fp);

	for ( ; adjust > 0; cnt++, adjust--)
		putc(fill, fp);

	return cnt;
}

/* TODO: control overflow in cnt */
int
vfprintf(FILE * restrict fp, const char *fmt, va_list va)
{
	int *p, ch, n, flags, width, fill, cnt = 0;
	char *s;
	wchar_t *ws;
	struct conv conv;
	char buf[BUF_SIZ+1];
	wchar_t wbuf[2];

	while ((ch = *fmt++) != '\0') {
		if (ch != '%') {
			putc(ch, fp);
			++cnt;
			continue;
		}

		fill = ' ';
		flags = width =  0;
		conv.prec = -1;
		conv.base = 10;
		conv.sign = '\0';
		conv.digs = "0123456789ABCDEFX";

flags:
		switch (*fmt++) {
		case '%':
			putc('%', fp);
			++cnt;
			continue;
		case ' ':
			conv.sign = ' ';
			goto flags;
		case '+':
			conv.sign = '+';
			goto flags;
		case '#':
			flags |= ALTFORM;
			goto flags;
		case '.':
			if (*fmt == '*') {
				fmt++;
				n = va_arg(va, int);
			} else {
				for (n = 0; isdigit(ch = *fmt); fmt++)
					n = n * 10 + ch - '0';
			}
			if (n > BUF_SIZ)
				n = BUF_SIZ;
			if (n > 0)
				conv.prec = n;
			goto flags;
		case '-':
			flags |= LADJUST;
			goto flags;
		case '*':
			n = va_arg(va, int);
			if (n < 0) {
				flags |= LADJUST;
				n = -n;
			}
			width = n;
			goto flags;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			--fmt;
			for (n = 0; isdigit(ch = *fmt); ++fmt)
				n = n * 10 + ch - '0';
			width = n;
			goto flags;
		case '0':
			fill = '0';
			goto flags;
		case 'l':
			flags += LONG;
			goto flags;
		case 'h':
			flags += SHORT;
			goto flags;
		case 'c':
			if (flags & LONG) {
				wbuf[0] = va_arg(va, wint_t);
				wbuf[1] = '\0';
				ws = wbuf;
				goto wstrout;
			} else {
				buf[0] = va_arg(va, int);
				buf[1] = '\0';
				s = buf;
				goto strout;
			}
		case 'j':
			flags |= INTMAX;
			goto flags;
		case 't':
			flags |= PTRDIFF;
			goto flags;
		case 'z':
			flags |= SIZET;
			goto flags;
		case 'u':
			flags |= UNSIGNED;
		case 'i':
		case 'd':
		numeric10:
			conv.base = 10;
			goto numeric;
		case 'p':
			flags |= VOIDPTR | ALTFORM;
			goto numeric16;
		case 'x':
			conv.digs = "0123456789abcdefx";
		case 'X':
		numeric16:
			conv.base = 16;
			flags |= UNSIGNED;
			goto numeric;
		case 'o':
			conv.base = 8;
			flags |= UNSIGNED;
		numeric:
			s = numtostr(getnum(va, flags, &conv.sign),
			             flags,
			             &conv,
			             &buf[BUF_SIZ]);
			goto strout;
		case 'L':
		case 'a':
		case 'A':
		case 'e':
		case 'E':
		case 'f':
		case 'g':
		case 'G':
			/* TODO */
		case 's':
			if ((flags & LONG) == 0){
				s = va_arg(va, char *);
				goto strout;
			}
			ws = va_arg(va, wchar_t *);
		wstrout:
			if (flags & LADJUST)
				width = -width;
			cnt += wstrout(ws, width, fill, fp);
			break;
		strout:
			if (flags & LADJUST)
				width = -width;
			cnt += strout(s, width, fill, fp);
			break;
		case 'n':
			setcnt(va, flags, cnt);
			break;
		case '\0':
			goto end_format;
		}
	}
end_format:
	return (ferror(fp)) ? EOF : cnt;
}
