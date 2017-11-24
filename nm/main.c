static char sccsid[] = "@(#) ./nm/main.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/arg.h"
#include "../inc/scc.h"
#include "../inc/myro.h"
#include "../inc/ar.h"

char *argv0;
int radix = 16;

void
fdie(char *fname)
{
	die("nm: %s: %s", fname, strerror(errno));
}

static int
myrofile(char *fname, FILE *fp)
{
	char magic[MYROMAGIC_SIZ];

	rewind(fp);
	fread(magic, sizeof(magic), 1, fp);
	if (ferror(fp))
		fdie(fname);
	return strncmp(magic, MYROMAGIC, MYROMAGIC_SIZ) == 0;
}

static int
arfile(char *fname, FILE *fp)
{
	char magic[ARMAGIC_SIZ];

	rewind(fp);
	fread(magic, sizeof(magic), 1, fp);
	if (ferror(fp))
		fdie(fname);
	return strncmp(magic, ARMAGIC, ARMAGIC_SIZ) == 0;
}

static void
nm(char *fname, char *member, FILE *fp)
{
	struct myrohdr hdr;
	size_t n;

	rewind(fp);
	if (rdmyrohdr(fp, &hdr) == EOF)
		fdie(fname);
	if (strncmp(hdr.magic, MYROMAGIC, MYROMAGIC_SIZ))
	n = hdr.symsize / MYROSYM_SIZ;
	if (n == 0) {
		fprintf(stderr, "nm: %s: no name list\n", member);
		return;
	}
}

void
doit(char *fname)
{
	FILE *fp;
	char magic[20];

	if ((fp = fopen(fname, "rb")) == NULL)
		fdie(fname);

	if (myrofile(fname, fp))
		nm(fname, fname, fp);
	else if (arfile(fname, fp))
		/* run over the members */;
	else
		die("nm: %s: File format not recognized", fname);

	if (fclose(fp) == EOF)
		fdie(fname);
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
		doit("a.out");
	} else {
		while (argc-- > 0)
			doit(*++argv);
	}

	return 0;
}
