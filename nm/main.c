static char sccsid[] = "@(#) ./nm/main.c";

#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/arg.h"
#include "../inc/scc.h"
#include "../inc/myro.h"
#include "../inc/ar.h"

char *argv0;
int radix = 16;

static int
myrofile(char *fname, FILE *fp)
{
	char magic[MYROMAGIC_SIZ];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, sizeof(magic), 1, fp);
	if (!ferror(fp)) {
		if (!strncmp(magic, MYROMAGIC, MYROMAGIC_SIZ))
			return 1;
	}

	fsetpos(fp, &pos);
	return 0;
}

static int
arfile(char *fname, FILE *fp)
{
	char magic[ARMAGIC_SIZ];
	fpos_t pos;

	fgetpos(fp, &pos);
	fread(magic, sizeof(magic), 1, fp);

	if (!ferror(fp)) {
		if (!strncmp(magic, ARMAGIC, ARMAGIC_SIZ))
			return 1;
	}

	fsetpos(fp, &pos);
	return 0;
}

static void
nm(char *fname, char *member, FILE *fp)
{
	struct myrohdr hdr;
	struct myrosym *syms;
	size_t n, siz;

	if (rdmyrohdr(fp, &hdr) < 0) {
		fprintf(stderr, "nm: %s: incorrect header\n", member);
		return;
	}

	if (hdr.symsize / MYROSYM_SIZ > SIZE_MAX)
		goto too_big;

	n = hdr.symsize / MYROSYM_SIZ;
	if (n == 0) {
		fprintf(stderr, "nm: %s: no name list\n", member);
		return;
	}

	if (n > SIZE_MAX / sizeof(struct myrosym))
		goto too_big;

	siz = n * sizeof(struct myrosym);
	syms = xmalloc(n);

	while (n--)
		;

	return;

too_big:
	fprintf(stderr, "nm: %s: too big symbol table\n", member);
	return;
}

static void
ar(char *fname, FILE *fp)
{
	struct arhdr hdr;
	long pos;

	while (rdarhdr(fp, &hdr) != EOF) {
		if ((pos = ftell(fp)) & 1)
			++pos;
		if (myrofile(fname, fp)) {
			nm(fname, hdr.name, fp);
		} else {
			fprintf(stderr,
			        "nm: skipping member %s in archive %s\n",
			        hdr.name, fname);
		}
		fseek(fp, pos, SEEK_SET);
	}
}

void
doit(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "rb")) == NULL)
		goto file_error;

	if (myrofile(fname, fp))
		nm(fname, fname, fp);
	else if (arfile(fname, fp))
		ar(fname, fp);
	else
		fprintf(stderr, "nm: %s: File format not recognized", fname);

	if (fclose(fp) != EOF)
		return;

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
		doit("a.out");
	} else {
		while (argc-- > 0)
			doit(*++argv);
	}

	return 0;
}
