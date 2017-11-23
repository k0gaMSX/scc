static char sccsid[] = "@(#) ./as/myro.c";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

struct myrohdr {
	unsigned long format;
	unsigned long long entry;
	unsigned long long strsize;
	unsigned long long secsize;
	unsigned long long symsize;
	unsigned long long relsize;
};

struct myrosect {
	unsigned long name;
	unsigned short flags;
	unsigned char fill;
	unsigned char aligment;
	unsigned long long offset;
	unsigned long long len;
};

struct myrosym {
	unsigned long name;
	unsigned long type;
	unsigned char section;
	unsigned char flags;
	unsigned long long offset;
	unsigned long long len;
};

struct myrorel {
	unsigned long id;
	unsigned char flags;
	unsigned char size;
	unsigned char nbits;
	unsigned char shift;
	unsigned long long offset;
};

static Reloc *relocs;
static size_t relcap, relsiz;

static void
writehdr(FILE *fp, struct myrohdr *hdr)
{
	unsigned char buf[sizeof(*hdr)];
	size_t len;

	len = lpack(buf, "lqqqqq",
	            hdr->format,
	            hdr->entry,
	            hdr->strsize,
	            hdr->secsize,
	            hdr->symsize,
	            hdr->relsize);
	fwrite(buf, len, 1, fp);
}

static size_t
writesec(FILE *fp, struct myrosect *sect)
{
	unsigned char buf[sizeof(*sect)];
	size_t len;

	len = lpack(buf, "lsccqq",
	            sect->name,
	            sect->flags,
	            sect->fill,
	            sect->aligment,
	            sect->offset,
	            sect->len);
	fwrite(buf, len, 1, fp);

	return len;
}

static size_t
writestrings(FILE *fp)
{
	size_t off = 0;
	size_t len;
	Symbol *sym;
	Section *sp;
	String str;

	fputs("z80-scc", fp);
	off = 7;

	for (sym = symlist; sym; sym = sym->next) {
		str = sym->name;
		len = strlen(str.buf) + 1;
		fwrite(str.buf, len, 1, fp);
		str.offset = off;
		off += len;
	}

	for (sp = seclist; sp; sp = sp->next) {
		str = sp->name;
		len = strlen(str.buf) + 1;
		fwrite(str.buf, len, 1, fp);
		str.offset = off;
		off += len;
	}

	return off;
}

static size_t
writesections(FILE *fp)
{
	Section *sp;
	size_t off = 0;
	struct myrosect sect;

	for (sp = seclist; sp; sp = sp->next) {
		sect.name = sp->name.offset;
		sect.flags = 0;
		sect.fill = 0;
		sect.aligment = 0;
		sect.offset = off;
		sect.len = 0;
		off += writesec(fp, &sect);
	}

	return off;
}

static size_t
writesym(FILE *fp, struct myrosym *sym)
{
	unsigned char buf[sizeof(*sym)];
	size_t len;

	len = lpack(buf, "llccqq",
	            sym->name,
	            sym->type,
	            sym->section,
	            sym->flags,
	            sym->offset,
	            sym->len);
	fwrite(buf, len, 1, fp);

	return len;
}

static size_t
writesymbols(FILE *fp)
{
	Symbol *sym;
	size_t off = 0;
	struct myrosym symbol;

	for (sym = symlist; sym; sym = sym->next) {
		symbol.name = sym->name.offset;
		symbol.type = -1;
		symbol.section = -1;
		symbol.flags = 0;
		symbol.offset = off;
		symbol.len = 0;
		off += writesym(fp, &symbol);
	}

	return off;
}

static size_t
writerel(FILE *fp, struct myrorel *rel)
{
	unsigned char buf[sizeof(*rel)];
	size_t len;

	len = lpack(buf, "lccccq",
	            rel->id,
	            rel->flags,
	            rel->size,
	            rel->nbits,
	            rel->shift,
	            rel->offset);
	return len;
}

static size_t
writerelocs(FILE *fp)
{
	Reloc *bp, *lim;
	size_t off = 0;
	struct myrorel reloc;

	lim = &relocs[relsiz];
	for (bp = relocs; bp < lim; ++bp) {
		reloc.id = 0;
		reloc.flags = bp->flags;
		reloc.size = bp->size;
		reloc.nbits = bp->nbits;
		reloc.shift = bp->shift;
		reloc.offset = bp->offset;
		off += writerel(fp, &reloc);
	}
	return off;
}

static void
writedata(FILE *fp)
{
	Section *sp;

	for (sp = seclist; sp; sp = sp->next) {
		if (!sp->mem)
			continue;
		fwrite(sp->mem, sp->max - sp->base, 1, fp);
	}
}

void
writeout(char *name)
{
	FILE *fp;
	long hdrpos;
	struct myrohdr hdr = {0};

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	fputs("uobj", fp);
	hdrpos = ftell(fp);
	writehdr(fp, &hdr);
	hdr.strsize = writestrings(fp);
	hdr.secsize = writesections(fp);
	hdr.symsize = writesymbols(fp);
	hdr.relsize = writerelocs(fp);
	writedata(fp);

	fseek(fp, hdrpos, SEEK_SET);
	writehdr(fp, &hdr);

	if (fclose(fp))
		die("error writing the output file");
}

void
reloc(Symbol *sym,
      unsigned flags, unsigned size, unsigned nbits, unsigned shift)
{
	size_t tmp;
	Reloc *p;

	if (relcap == relsiz) {
		tmp = ((relcap + 1) * 3) / 2;
		if ((p = realloc(relocs, tmp * sizeof(Reloc))) == NULL) {
			tmp = relcap + 1;
			p = xrealloc(relocs, tmp * sizeof(Reloc));
		}
		relcap = tmp;
		relocs = p;
	}

	p = &relocs[relsiz++];
	p->sym = sym;
	p->flags = flags;
	p->size = size;
	p->nbits = nbits;
	p->shift = shift;
	p->offset = cursec->pc - cursec->base;
}