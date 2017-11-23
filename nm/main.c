static char sccsid[] = "@(#) ./nm/main.c";

#include <stdio.h>
#include <stdlib.h>

#include "../inc/arg.h"
#include "../inc/scc.h"

char *argv0;

void
usage(void)
{
	fputs("nm [-APv][-efox][ -g| -u][-t format] file...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 'A':
	case 'e':
	case 'f':
	case 'g':
	case 'o':
	case 'u':
	case 'v':
	case 'x':
	/* case 't': */
		;
	default:
		usage();
	} ARGEND

	return 0;
}
