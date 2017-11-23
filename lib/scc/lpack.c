#include <stdarg.h>

#include "../../inc/scc.h"

int
lpack(unsigned char *dst, char *fmt, ...)
{
	unsigned char *bp;
	unsigned s;
	unsigned long l;
	unsigned long long q;
	va_list va;

	bp = dst;
	va_start(va, fmt);
	while (*fmt) {
		switch (*fmt++) {
		case 'c':
			*bp++ = va_arg(va, unsigned);
			break;
		case 's':
			s = va_arg(va, unsigned);
			*bp++ = s >> 8;
			*bp++ = s;
			break;
		case 'l':
			l = va_arg(va, unsigned long);
			*bp++ = l >> 24;
			*bp++ = l >> 16;
			*bp++ = l >> 8;
			*bp++ = l;
			break;
		case 'q':
			q = va_arg(va, unsigned long long);
			*bp++ = q >> 56;
			*bp++ = q >> 48;
			*bp++ = q >> 40;
			*bp++ = q >> 32;
			*bp++ = q >> 24;
			*bp++ = q >> 16;
			*bp++ = q >> 8;
			*bp++ = q;
			break;
		default:
			va_end(va);
			return -1;
		}
	}
	va_end(va);

	return dst - bp;
}
