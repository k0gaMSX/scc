static char sccsid[] = "@(#) ./as/symbol.c";

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define HASHSIZ 64
#define NALLOC  10

static Section abss = {
	.name  = "abs",
	.flags = TABS|SREAD|SWRITE,
};

static Section bss = {
	.name  = "bss",
	.flags = TBSS|SRELOC|SREAD|SWRITE,
	.next  = &abss,
};

static Section data = {
	.name  = "data",
	.flags = TDATA|SRELOC|SREAD|SWRITE|SFILE,
	.next  = &bss,
};

static Section text = {
	.name  = "text",
	.flags = TTEXT|SRELOC|SFILE,
	.next  = &data,
};

Section *cursec = &text, *headp = &text;

int pass;

static Symbol *hashtbl[HASHSIZ];
static Alloc *tmpalloc;

Symbol *linesym;

#ifndef NDEBUG
void
dumpstab(char *msg)
{
	Symbol **bp, *sym;

	fprintf(stderr, "%s\n", msg);
	for (bp = hashtbl; bp < &hashtbl[HASHSIZ]; ++bp) {
		if (*bp == NULL)
			continue;

		fprintf(stderr, "[%d]", (int) (bp - hashtbl));
		for (sym = *bp; sym; sym = sym->next) {
			fprintf(stderr, " -> %s:%0X:%0X",
			       sym->name, sym->flags, sym->argtype);
		}
		putc('\n', stderr);
	}
}
#endif

Symbol *
lookup(char *name, int type)
{
	unsigned h;
	Symbol *sym, **list;
	int c, symtype;
	char *t, *s;

	h = 0;
	for (s = name; c = *s; ++s)
		h = h*33 ^ c;
	h &= HASHSIZ-1;

	c = toupper(*name);
	list = &hashtbl[h];
	for (sym = *list; sym; sym = sym->next) {
		t = sym->name;
		if (c != toupper(*t) || casecmp(t, name))
			continue;
		symtype = sym->flags & TMASK;
		if (symtype != TUNDEF && symtype != type)
			continue;
		return sym;
	}

	sym = xmalloc(sizeof(*sym));
	sym->name = xstrdup(name);
	sym->flags = (cursec->flags & TMASK) | FLOCAL | FUNDEF | type;
	sym->desc = 0;
	sym->value = 0;
	sym->next = *list;
	*list = sym;

	return sym;
}

Symbol *
deflabel(char *name)
{
	static Symbol *cursym;
	Symbol *sym;
	char label[MAXSYM+1];

	if (*name == '.') {
		int r;

		if (!cursym) {
			error("local label '%s' without global label", name);
			return NULL;
		}
		r = snprintf(label, sizeof(label),
		             "%s%s",
		             cursym->name, name);
		if (r == sizeof(label)) {
			error("local label '%s' in '%s' produces too long symbol",
			      name, cursym->name);
			return NULL;
		}
		name = label;
	}

	sym = lookup(name, TUNDEF);
	if (pass == 1 && (sym->flags & FUNDEF) == 0)
		error("redefinition of label '%s'", name);
	sym->flags &= ~FUNDEF;
	sym->value = cursec->curpc;

	if (*name != '.')
		cursym = sym;
	return sym;
}

int
toobig(Node *np, int type)
{
	/* TODO */
	return 0;
}

char *
pack(TUINT v, int n, int inc)
{
	static char buf[sizeof(TUINT)];
	int idx;

	idx = (inc < 0) ? n-1 : 0;
	while (n--) {
		buf[idx] = v;
		idx += inc;
		v >>= 8;
	}

	if (v)
		error("overflow in immediate value");
	return buf;
}

static void
incpc(int siz)
{
	TUINT pc, curpc;

	pc = cursec->pc;
	curpc = cursec->curpc;

	cursec->curpc += siz;
	cursec->pc += siz;

	if (pass == 2)
		return;

	if (cursec->pc > cursec->max)
		cursec->max = cursec->pc;

	if (pc > cursec->pc ||
	    curpc > cursec->curpc ||
	    cursec->curpc > maxaddr ||
	    cursec->pc > maxaddr) {
		die("address overflow");
	}
}


static void
isect(Section *sec)
{
	TUINT siz;

	sec->curpc = sec->pc = sec->base;
	if (pass == 1 || !(sec->flags & SFILE))
		return;

	siz = sec->max - sec->base;
	if (siz > SIZE_MAX)
		die("out of memory");
	sec->mem = xmalloc(sec->max - sec->base);
}

Section *
section(char *name)
{
	Section *sec;

	for (sec = headp; sec; sec = sec->next) {
		if (!strcmp(sec->name, name))
			break;
	}
	if (!sec) {
		sec = xmalloc(sizeof(*sec));
		sec->name = xstrdup(name);
		sec->base = sec->max = sec->pc = sec->curpc = 0;
		sec->next = headp;
		sec->flags = SRELOC|SREAD|SWRITE|SFILE;
	}
	return cursec = sec;
}

void
isections(void)
{
	Section *sec;

	for (sec = headp; sec; sec = sec->next)
		isect(sec);
}

void
emit(Section *sec, char *bytes, int n)
{
	if (sec->mem)
		memcpy(&sec->mem[sec->pc - sec->base], bytes, n);
	incpc(n);
}

Symbol *
tmpsym(TUINT val)
{
	Symbol *sym;

	if (!tmpalloc)
		tmpalloc = alloc(sizeof(*sym), NALLOC);
	sym = new(tmpalloc);
	sym->value = val;
	sym->flags = TABS;

	return sym;
}

void
killtmp(void)
{
	if (!tmpalloc)
		return;
	dealloc(tmpalloc);
	tmpalloc = NULL;
}

void
writeout(char *name)
{
	FILE *fp;
	Section *secp;

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	for (secp = headp; secp; secp = secp->next) {
		if (!secp->mem)
			continue;
		fwrite(secp->mem, secp->max - secp->base, 1, fp);
	}

	if (fclose(fp))
		die("error writing the output file");
}
