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
char *strings;
static int radix = 16;
static int Pflag;
static int Aflag;
static int vflag;
static int gflag;
static int uflag;
static int archflag;

static int
object(char *fname, FILE *fp)
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
archive(char *fname, FILE *fp)
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

static int
cmp(const void *p1, const void *p2)
{
	const struct myrosym *s1 = p1, *s2 = p2;

	if (vflag)
		return s1->offset - s2->offset;
	else
		return strcmp(strings + s1->name, strings + s2->name);
}

static int
typeof(struct myrosym *sym)
{
	return 'U';
}

static void
print(char *file, char *member, struct myrosym *sym, FILE *fp)
{
	char *fmt, *name = strings + sym->name;
	int type = typeof(sym);

	if (uflag && type != 'U')
		return;
	if (gflag && type != 'A' && type != 'B' && type != 'D')
		return;

	if (Aflag)
		fprintf(fp, (archflag) ? "%s[%s]: " : "%s: ", file, member);
	if (Pflag) {
		fprintf(fp, "%s %c", name, type);
		if (type != 'U') {
			if (radix == 8)
				fmt = "%llo %llo";
			else if (radix == 10)
				fmt = "%llu %llu";
			else
				fmt = "%llx %llx";
			fprintf(fp, fmt, sym->offset, sym->len);
		}
	} else {
		if (type == 'U')
			fmt = "                ";
		else if (radix == 8)
			fmt = "%016.16llo";
		else if (radix == 8)
			fmt = "%016.16lld";
		else
			fmt = "%016.16llx";
		fprintf(fp, fmt, sym->offset);
		fprintf(fp, " %c %s", type, name);
	}
	putc('\n', fp);
}

static void
nm(char *fname, char *member, FILE *fp)
{
	struct myrohdr hdr;
	struct myrosym *syms = NULL;
	size_t n, i;
	long off;

	strings = NULL;
	if (rdmyrohdr(fp, &hdr) < 0) {
		fprintf(stderr, "nm: %s: incorrect header\n", member);
		return;
	}

	n = hdr.symsize / MYROSYM_SIZ;
	if (n == 0) {
		fprintf(stderr, "nm: %s: no name list\n", member);
		return;
	}
	if (n > SIZE_MAX / sizeof(struct myrosym) ||
	    hdr.symsize / MYROSYM_SIZ > SIZE_MAX ||
	    hdr.strsize > SIZE_MAX) {
		goto offset_overflow;
	}

	syms = xmalloc(n * sizeof(struct myrosym));
	strings = xmalloc(hdr.strsize);
	fread(strings, hdr.strsize, 1, fp);
	if (feof(fp))
		goto free_arrays;
	if ((off = ftell(fp)) < 0)
		return;
	if (off > LONG_MAX - hdr.secsize)
		goto offset_overflow;
	off += hdr.secsize;

	if (fseek(fp, off, SEEK_SET) < 0)
		goto free_arrays;

	for (i = 0; i < n; ++i) {
		if (rdmyrosym(fp, &syms[i]) < 0)
			goto free_arrays;
		if (syms[i].name >= hdr.strsize)
			goto offset_overflow;
	}
	qsort(syms, n, sizeof(*syms), cmp);
	for (i = 0; i < n; ++i)
		print(fname, member, &syms[i], fp);


free_arrays:
	free(syms);
	free(strings);
	return;

offset_overflow:
	fprintf(stderr, "nm: %s: overflow in headers of archive\n",
		fname);
	goto free_arrays;
}

static void
ar(char *fname, FILE *fp)
{
	struct arhdr hdr;
	long pos;

	archflag = 1;
	while (rdarhdr(fp, &hdr) != EOF) {
		pos = ftell(fp);
		if (pos == -1 || pos > LONG_MAX - hdr.size) {
			fprintf(stderr,
			        "nm: %s: overflow in size of archive\n",
			        fname);
			return;
		}
		pos += hdr.size;
		if (hdr.size & 1)
			++pos;

		if (object(fname, fp)) {
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

	archflag = 0;
	if ((fp = fopen(fname, "rb")) == NULL)
		goto file_error;

	if (object(fname, fp))
		nm(fname, fname, fp);
	else if (archive(fname, fp))
		ar(fname, fp);
	else
		fprintf(stderr, "nm: %s: File format not recognized\n", fname);

	if (fclose(fp) != EOF)
		return;

file_error:
	die("nm: %s: %s", fname, strerror(errno));
}

void
usage(void)
{
	fputs("nm [-APv][ -g| -u][-t format] [file...]\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	char *t;

	ARGBEGIN {
	case 'P':
		Pflag = 1;
		break;
	case 'A':
		Aflag = 1;
		break;
	case 'g':
		gflag = 1;
		break;
	case 'u':
		uflag = 1;
		break;
	case 'v':
		vflag = 1;
		break;
	case 't':
		t = EARGF(usage());
		if (!strcmp(t, "o"))
			radix = 8;
		else if (!strcmp(t, "d"))
			radix = 10;
		else if (!strcmp(t, "x"))
			radix = 16;
		else
			usage();
		break;
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
