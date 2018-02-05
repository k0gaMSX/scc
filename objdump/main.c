
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

struct obj_info {
	char *fname;
	FILE *fp;
	struct myrohdr hdr;
	fpos_t strpos;
	fpos_t secpos;
	fpos_t sympos;
	fpos_t relpos;
	fpos_t datapos;
};

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

static int
printhdr(struct obj_info *obj)
{
	struct myrohdr *hdr = &obj->hdr;

	printf("header:\n"
	       " magic: %02x %02x %02x %02x \"%4.4s\"\n"
	       " format: %lu (\"%s\")\n"
	       " entry: %llu\n"
	       " string table size: %llu\n"
	       " section table size: %llu\n"
	       " symbol table size: %llu\n"
	       " relocation table size: %llu\n",
	       hdr->magic[0], hdr->magic[1],
	       hdr->magic[2], hdr->magic[3],
	       hdr->magic,
	       hdr->format, getstring(hdr->format),
	       hdr->entry,
	       hdr->strsize,
	       hdr->secsize,
	       hdr->symsize,
	       hdr->relsize);
	return 0;
}

static int
printstrings(struct obj_info *obj)
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
			printf(" [%zd] \"%s\"\n", begin, s);
			s = NULL;
		}
	}
	return 0;
}

static char *
sectflags(struct myrosect *sec)
{
	static char flags[10];
	char *s = flags + sizeof(flags);

	if (sec->flags & MYROSEC_LOAD)
		*--s = 'L';
	if (sec->flags & MYROSEC_FILE)
		*--s = 'F';
	if (sec->flags & MYROSEC_ABS)
		*--s = 'A';
	if (sec->flags & MYROSEC_EXEC)
		*--s = 'X';
	if (sec->flags & MYROSEC_WRITE)
		*--s = 'W';
	if (sec->flags & MYROSEC_READ)
		*--s = 'R';
	return s;
}

static int
printsections(struct obj_info *obj)
{
	unsigned long long n, i;
	struct myrosect sect;
	struct myrohdr *hdr = &obj->hdr;

	printf("sections:\n"
	       " [Nr]\t%s\t%-16s\t%-16s\t%s\t%s\t%s\n",
	       "Name",
	       "Offset",
	       "Size",
	       "Fill",
	       "Align",
	       "Flags");

	n = hdr->secsize / MYROSECT_SIZ;
	for (i = 0; i < n; ++i) {
		if (rdmyrosec(obj->fp, &sect) < 0)
			return -1;
		printf(" [%2llu]\t%s\t%016llX\t%016llX\t%02X\t%u\t%s\n",
		       i,
		       getstring(sect.name),
		       sect.offset,
		       sect.len,
		       sect.fill,
		       sect.aligment,
		       sectflags(&sect));
	}
	return 0;
}

static char *
symflags(struct myrosym *sym)
{
	static char flags[10];
	char *s = flags + sizeof(flags);

	if (sym->flags & MYROSYM_COMMON)
		*--s = 'C';
	if (sym->flags & MYROSYM_EXTERN)
		*--s = 'G';
	if (sym->flags & MYROSYM_UNDEF)
		*s-- = 'U';
	return s;
}

static int
printsymbols(struct obj_info *obj)
{
	unsigned long long n, i;
	struct myrosym sym;
	struct myrohdr *hdr = &obj->hdr;

	printf("symbols:\n"
	       " [Nr]\t%s\t%-16s\t%s\t%s\t%s\t%s\n",
	       "Name",
	       "Value",
	       "Section",
	       "Flags",
	       "Size",
	       "Type");
	n = hdr->symsize / MYROSYM_SIZ;
	for (i = 0; i < n; ++i) {
		if (rdmyrosym(obj->fp, &sym) < 0)
			return -1;
		printf(" [%2llu]\t%s\t%016llX\t%u\t%s\t%llu\t%s\n",
		       i,
		       getstring(sym.name),
		       sym.offset,
		       sym.section,
		       symflags(&sym),
		       sym.len,
		       getstring(sym.type));
	}
	return 0;
}

static int
printrelocs(struct obj_info *obj)
{
	unsigned long long n, i;
	struct myrorel rel;
	struct myrohdr *hdr = &obj->hdr;

	printf("relocs:\n"
	       " [Nr]\t%-16s\tType\tId\tSize\tNbits\tShift\n",
	       "Offset");
	n = hdr->relsize / MYROREL_SIZ;
	for (i = 0; i < n; ++i) {
		if (rdmyrorel(obj->fp, &rel) < 0)
			return -1;
		printf(" [%2llu]\t%016llX\t%s\t%lu\t%u\t%u\t%u\n",
		       i,
		       rel.offset,
		       (rel.id & 1<<31) ? "section" : "symbol",
		       rel.id & ~(1<<31),
		       rel.size,
		       rel.nbits,
		       rel.shift);
	}
	return 0;
}

static int
printdata(struct obj_info *obj)
{
	unsigned long long off;
	int c, i, j;

	puts("data:");
	for (off = 0; ; off += 32) {
		printf(" %016llX:", off);
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 8; j++) {
				if ((c = getc(obj->fp)) == EOF)
					goto exit_loop;
				printf(" %02X", c);
			}
			putchar('\t');
		}
		putchar('\n');
	}

exit_loop:
	putchar('\n');
	return (ferror(obj->fp)) ? -1 : 0;
}

void
dump(char *fname, FILE *fp)
{
	struct obj_info obj;
	struct myrohdr *hdr;

	obj.fname = fname;
	obj.fp = fp;
	hdr = &obj.hdr;

	if (rdmyrohdr(obj.fp, hdr) < 0)
		goto wrong_file;
	if (strncmp(hdr->magic, MYROMAGIC, MYROMAGIC_SIZ)) {
		fprintf(stderr,
		        "objdump: %s: File format not recognized\n",
		        fname);
		return;
	}
	puts(fname);
	if (hdr->strsize > SIZE_MAX) {
		fprintf(stderr,
			"objdump: %s: overflow in header\n",
			fname);
			return;
	}
	strsiz = hdr->strsize;

	if (strsiz > 0) {
		strings = xmalloc(strsiz);
		fread(strings, strsiz, 1, fp);
		if (feof(fp))
			goto wrong_file;
	}

	if (printhdr(&obj) < 0)
		goto wrong_file;
	if (printstrings(&obj) < 0)
		goto wrong_file;
	if (printsections(&obj) < 0)
		goto wrong_file;
	if (printsymbols(&obj) < 0)
		goto wrong_file;
	if (printrelocs(&obj) < 0)
		goto wrong_file;
	if (printdata(&obj) < 0)
		goto wrong_file;
	return;

wrong_file:
	fprintf(stderr,
		"objdump: %s: %s\n",
		fname, strerror(errno));
}

void
doit(char *fname)
{
	FILE *fp;

	if ((fp = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "objdump: %s: %s\n", fname, strerror(errno));
		return;
	}
	dump(fname, fp);
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

	if (argc == 0)
		doit("a.out");
	else while (*argv) {
		free(strings);
		strings = NULL;
		doit(*argv++);
	}

	if (fclose(stdout) == EOF)
		die("objdump: stdout: %s", strerror(errno));

	return 0;
}
