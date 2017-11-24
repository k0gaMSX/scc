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
	fpos_t pos;

	if (fgetpos(fp, &pos) < 0)
		fdie(fname);

	fread(magic, sizeof(magic), 1, fp);
	if (ferror(fp))
		fdie(fname);

	if (!strncmp(magic, MYROMAGIC, MYROMAGIC_SIZ))
		return 1;

	if (fsetpos(fp, &pos) < 0)
		fdie(fname);
	return 0;
}

static int
arfile(char *fname, FILE *fp)
{
	char magic[ARMAGIC_SIZ];
	fpos_t pos;

	fread(magic, sizeof(magic), 1, fp);
	if (ferror(fp))
		fdie(fname);

	if (!strncmp(magic, ARMAGIC, ARMAGIC_SIZ))
		return 1;

	if (fsetpos(fp, &pos) < 0)
		fdie(fname);
	return 0;
}

static void
nm(char *fname, char *member, FILE *fp)
{
	struct myrohdr hdr;
	size_t n;

	if (rdmyrohdr(fp, &hdr) < 0)
		fdie(fname);

	n = hdr.symsize / MYROSYM_SIZ;
	if (n == 0) {
		fprintf(stderr, "nm: %s: no name list\n", member);
		return;
	}
}

static void
ar(char *fname, FILE *fp)
{
	struct arhdr hdr;

	while (rdarhdr(fp, &hdr) != EOF) {
		if (myrofile(fname, fp)) {
			nm(fname, hdr.name, fp);
		} else {
			fprintf(stderr,
			        "nm: skipping member %s in archive %s\n",
			        hdr.name, fname);
		}
	}
}

void
doit(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "rb")) == NULL)
		fdie(fname);

	if (myrofile(fname, fp))
		nm(fname, fname, fp);
	else if (arfile(fname, fp))
		ar(fname, fp);
	else
		fprintf(stderr, "nm: %s: File format not recognized", fname);

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
