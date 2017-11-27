static char sccsid[] = "@(#) ./as/myro.c";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "../inc/myro.h"
#include "as.h"

#define FORMAT "z80-scc"

static Reloc *relocs;
static size_t relcap, relsiz;

static size_t
writestrings(FILE *fp)
{
	size_t off = 0;
	size_t len;
	Symbol *sym;
	Section *sp;
	String *str;

	fwrite(FORMAT, sizeof(FORMAT), 1, fp);
	off = sizeof(FORMAT);

	for (sym = symlist; sym; sym = sym->next) {
		if ((sym->flags & TMASK) == TREG)
			continue;
		str = &sym->name;
		len = strlen(str->buf) + 1;
		fwrite(str->buf, len, 1, fp);
		str->offset = off;
		off += len;
	}

	for (sp = seclist; sp; sp = sp->next) {
		str = &sp->name;
		len = strlen(str->buf) + 1;
		fwrite(str->buf, len, 1, fp);
		str->offset = off;
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
		off += wrmyrosec(fp, &sect);
	}

	return off;
}

static size_t
writesymbols(FILE *fp)
{
	Symbol *sym;
	size_t off = 0;
	struct myrosym symbol;

	for (sym = symlist; sym; sym = sym->next) {
		if ((sym->flags & TMASK) == TREG)
			continue;
		symbol.name = sym->name.offset;
		symbol.type = -1;
		symbol.section = -1;
		symbol.flags = 0;
		symbol.offset = off;
		symbol.len = 0;
		off += wrmyrosym(fp, &symbol);
	}

	return off;
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
		off += wrmyrorel(fp, &reloc);
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
	struct myrohdr hdr = { .magic = MYROMAGIC };

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	wrmyrohdr(fp, &hdr);
	hdr.strsize = writestrings(fp);
	hdr.secsize = writesections(fp);
	hdr.symsize = writesymbols(fp);
	hdr.relsize = writerelocs(fp);
	writedata(fp);

	fseek(fp, 0, SEEK_SET);
	wrmyrohdr(fp, &hdr);

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
