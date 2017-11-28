
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/arg.h"
#include "../inc/scc.h"
#include "../inc/myro.h"

char *argv0;
static char *strings;
static size_t strsiz;

static char *
getstring(unsigned long off)
{
	size_t n;

	if ((int32_t) off == -1)
		return "";
	if (off < SIZE_MAX) {
		for (n = off; n < strsiz && strings[n]; ++n)
			;
		if (n < strsiz)
			return &strings[off];
	}
	fprintf(stderr, "objdump: wrong string offset %lu\n", off);
	return "";
}

static void
printhdr(struct myrohdr *hdr)
{
	printf("header:\n"
	       "\tmagic: %02x%02x%02x%02x \"%4.4s\"\n"
	       "\tformat: %lu (\"%s\")\n"
	       "\tentry: %llu\n"
	       "\tstring table size: %llu\n"
	       "\tsection table size: %llu\n"
	       "\tsymbol table size: %llu\n"
	       "\trelocation table size: %llu\n",
	       hdr->magic[0], hdr->magic[1],
	       hdr->magic[2], hdr->magic[3],
	       hdr->magic,
	       hdr->format, getstring(hdr->format),
	       hdr->entry,
	       hdr->strsize,
	       hdr->secsize,
	       hdr->symsize,
	       hdr->relsize);
}

static void
printstrings(struct myrohdr *hdr)
{
	size_t off, begin;;
	char *s = NULL;

	puts("strings:");
	for (off = 0; off < strsiz; off++) {
		if (s == NULL) {
			s = &strings[off];
			begin = off;
		}
		if (strings[off] == '\0') {
			printf("\t[%zd] \"%s\"\n", begin, s);
			s = NULL;
		}
	}
}

static int
printsections(struct myrohdr *hdr, FILE *fp)
{
	unsigned long long n, i;
	struct myrosect sect;

	puts("sections:");
	n = hdr->secsize / MYROSECT_SIZ;
	for (i = 0; i < n; ++i) {
		if (rdmyrosec(fp, &sect) < 0)
			return -1;
		printf("\tname: %lu (\"%s\")\n"
		       "\tflags: %x\n"
		       "\tfill: %x\n"
		       "\taligment: %u\n"
		       "\toffset: %llu\n"
		       "\tlength: %llu\n\n",
		        sect.name, getstring(sect.name),
		        sect.flags,
		        sect.fill,
		        sect.aligment,
		        sect.offset,
		        sect.len);
	}
	return 0;
}

static int
printsymbols(struct myrohdr *hdr, FILE *fp)
{
	unsigned long long n, i;
	struct myrosym sym;

	puts("symbols:");
	n = hdr->symsize / MYROSYM_SIZ;
	for (i = 0; i < n; ++i) {
		if (rdmyrosym(fp, &sym) < 0)
			return -1;
		printf("\tname: %lu (\"%s\")\n"
		       "\ttype: %lu (\"%s\")\n"
		       "\tsection: %u\n"
		       "\tflags: %x\n"
		       "\toffset: %llu\n"
		       "\tlength: %llu\n\n",
		       sym.name, getstring(sym.name),
		       sym.type, getstring(sym.type),
		       sym.section,
		       sym.flags,
		       sym.offset,
		       sym.len);
	}
	return 0;
}

static int
printrelocs(struct myrohdr *hdr, FILE *fp)
{
	unsigned long long n, i;
	struct myrorel rel;

	puts("relocs:");
	n = hdr->relsize / MYROREL_SIZ;
	for (i = 0; i < n; ++i) {
		if (rdmyrorel(fp, &rel) < 0)
			return -1;
		printf("\tid: %lu\n"
		       "\tflags: %x\n"
		       "\tsize: %u\n"
		       "\tnbits: %u\n"
		       "\tshift: %u\n"
		       "\toffset: %llu\n",
		       rel.id,
		       rel.flags,
		       rel.size,
		       rel.nbits,
		       rel.shift,
		       rel.offset);
	}
	return 0;
}

static int
printdata(struct myrohdr *hdr, FILE *fp)
{
	unsigned long off;
	int c, i, j;

	puts("data:");
	for (off = 0; ; off += 32) {
		printf("\t%08x:", off);
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 16; j++) {
				if ((c = getc(fp)) == EOF)
					goto exit_loop;
				printf(" %02X", c);
			}
			putchar('\t');
		}
		putchar('\n');
	}

exit_loop:
	putchar('\n');
	return (ferror(fp)) ? -1 : 0;
}

void
dump(char *fname)
{
	FILE *fp;
	struct myrohdr hdr;

	puts(fname);
	if ((fp = fopen(fname, "rb")) == NULL)
		goto wrong_file;
	if (rdmyrohdr(fp, &hdr) < 0)
		goto wrong_file;
	if (hdr.strsize > SIZE_MAX) {
		fprintf(stderr,
			"objdump: %s: overflow in header\n",
			fname, strerror(errno));
			goto close_file;
	}
	strsiz = hdr.strsize;

	if (strsiz > 0) {
		strings = xmalloc(strsiz);
		fread(strings, strsiz, 1, fp);
		if (feof(fp))
			goto wrong_file;
	}

	printhdr(&hdr);
	printstrings(&hdr);
	if (printsections(&hdr, fp) < 0)
		goto wrong_file;
	if (printsymbols(&hdr, fp) < 0)
		goto wrong_file;
	if (printrelocs(&hdr, fp) < 0)
		goto wrong_file;
	if (printdata(&hdr, fp) < 0)
		goto wrong_file;

	goto close_file;

wrong_file:
	fprintf(stderr,
		"objdump: %s: %s\n",
		fname, strerror(errno));
close_file:
	if (fp)
		fclose(fp);
}

void
usage(void)
{
	fputs("usage: objdump file ...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	ARGBEGIN {
	default:
		usage();
	} ARGEND

	if (argc == 1)
		dump("a.out");
	else while (*++argv) {
		free(strings);
		strings = NULL;
		dump(*argv);
	}

	return 0;
}
