static char sccsid[] = "@(#) ./nm/main.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/arg.h"
#include "../inc/scc.h"
#include "../inc/myro.h"

char *argv0;
int radix = 16;

void
nm(char *fname)
{
	FILE *fp;
	struct myrohdr hdr;

	if ((fp = fopen(fname, "rb")) == NULL)
		goto file_error;
	if (readhdr(fp, &hdr) == EOF)
		goto file_error;
	if (strncmp(hdr.magic, MYROMAGIC, MYROMAGIC_SIZ))
		die("nm: %s: File format not recognized", fname);

file_error:
	die("nm: %s: %s", fname, strerror(errno));
}

void
usage(void)
{
	fputs("nm [-APv][ -g| -u][-t format] file...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	case 'A':
	case 'g':
	case 'u':
	case 'v':
	/* case 't': */
		;
	default:
		usage();
	} ARGEND

	if (argc == 0) {
		nm("a.out");
	} else {
		while (argc-- > 0)
			nm(*++argv);
	}

	return 0;
}
