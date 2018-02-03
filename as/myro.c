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
	int type;
	size_t off = 0;
	size_t len;
	Symbol *sym;
	Section *sp;
	String *str;

	fwrite(FORMAT, sizeof(FORMAT), 1, fp);
	off = sizeof(FORMAT);

	for (sym = symlist; sym; sym = sym->next) {
		if (sym->flags & FREG)
			continue;
		str = &sym->name;
		len = strlen(str->buf) + 1;
		fwrite(str->buf, len, 1, fp);
		str->offset = off;
		off += len;
	}

	return off;
}

static unsigned
getsecflags(Section *sp)
{
	unsigned flags = 0;

	if (sp->flags & SREAD)
		flags |= MYROSEC_READ;
	if (sp->flags & SWRITE)
		flags |= MYROSEC_WRITE;
	if (sp->flags & SFILE)
		flags |= MYROSEC_FILE;
	if (sp->flags & SEXEC)
		flags |= MYROSEC_EXEC;
	if (sp->flags & SLOAD)
		flags |= MYROSEC_LOAD;
	if (sp->flags & SABS)
		flags |= MYROSEC_ABS;
	return flags;
}

static size_t
writesections(FILE *fp)
{
	Section *sp;
	size_t off = 0;
	struct myrosect sect;
	unsigned id = 0;;

	for (sp = seclist; sp; sp = sp->next) {
		if (id == MYROMAXSEC)
			die("too many sections for a myro file");
		sp->id = id++;
		sect.name = sp->sym->name.offset;
		sect.flags = getsecflags(sp);
		sect.fill = sp->fill;
		sect.aligment = sp->aligment;
		sect.offset = off;
		sect.len = sp->max - sp->base;
		off += wrmyrosec(fp, &sect);
	}

	return off;
}

static unsigned
getsymflags(Symbol *sym)
{
	unsigned flags = 0;

	if (sym->flags & FCOMMON)
		flags |= MYROSYM_COMMON;
	if (sym->flags & FEXTERN)
		flags |= MYROSYM_EXTERN;
	if (!(sym->flags & FDEF))
		flags |= MYROSYM_UNDEF;
	return flags;
}

static size_t
writesymbols(FILE *fp)
{
	Symbol *sym;
	size_t off = 0;
	struct myrosym symbol;

	for (sym = symlist; sym; sym = sym->next) {
		if (sym->flags & (FREG|FSECT))
			continue;
		symbol.name = sym->name.offset;
		symbol.type = -1;
		symbol.section = sym->section->id;
		symbol.flags = getsymflags(sym);
		symbol.offset = sym->value;
		symbol.len = sym->size;
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

	if (pass == 1)
		return;

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
