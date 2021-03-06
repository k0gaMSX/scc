static char sccsid[] = "@(#) ./lib/scc/die.c";
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../inc/scc.h"

int failure;

void
die(const char *fmt, ...)
{
	failure = 1;
	va_list va;
	va_start(va, fmt);
	vfprintf(stderr, fmt, va);
	putc('\n', stderr);
	va_end(va);
	exit(1);
}
