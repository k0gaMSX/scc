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
	.name  = (String) {"abs"},
	.flags = SABS|SREAD|SWRITE|SFILE|SLOAD,
};

static Section bss = {
	.name  = (String) {"bss"},
	.flags = SREAD|SWRITE|SLOAD,
	.next  = &abss,
};

static Section data = {
	.name  = (String) {"data"},
	.flags = SREAD|SWRITE|SFILE|SLOAD,
	.next  = &bss,
};

static Section text = {
	.name  = (String) {"text"},
	.flags = SREAD|SEXEC|SLOAD|SFILE,
	.next  = &data,
};

Section *cursec = &text, *seclist = &text;

int pass;

static Symbol *hashtbl[HASHSIZ];
static Alloc *tmpalloc;

Symbol *linesym, *symlist, *symlast;

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
		for (sym = *bp; sym; sym = sym->hash) {
			fprintf(stderr, " -> %s:%0X:%0X",
			       sym->name.buf, sym->flags, sym->argtype);
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
	for (sym = *list; sym; sym = sym->hash) {
		t = sym->name.buf;
		if (c != toupper(*t) || casecmp(t, name))
			continue;
		symtype = sym->flags;
		if (((symtype | type) & FUNDEF) == 0 && symtype != type)
			continue;
		return sym;
	}

	sym = xmalloc(sizeof(*sym));
	sym->name = newstring(name);
	sym->flags = FRELOC | FUNDEF | type;
	sym->value = 0;
	sym->section = cursec;
	sym->hash = *list;
	sym->next = NULL;

	*list = sym;
	if (symlast)
		symlast->next = sym;
	symlast = sym;
	if (!symlist)
		symlist = sym;

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
		             cursym->name.buf, name);
		if (r == sizeof(label)) {
			error("local label '%s' in '%s' produces too long symbol",
			      name, cursym->name.buf);
			return NULL;
		}
		name = label;
	}

	sym = lookup(name, FUNDEF);
	if (pass == 1 && (sym->flags & FUNDEF) == 0)
		error("redefinition of label '%s'", name);
	if (cursec->flags & SABS)
		sym->flags &= ~FRELOC;
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

	for (sec = seclist; sec; sec = sec->next) {
		if (!strcmp(sec->name.buf, name))
			break;
	}
	if (!sec) {
		sec = xmalloc(sizeof(*sec));
		sec->name = newstring(name);
		sec->base = sec->max = sec->pc = sec->curpc = 0;
		sec->next = seclist;
		sec->flags = 0;
		sec->fill = 0;
		sec->aligment = 0;
	}
	return cursec = sec;
}

void
isections(void)
{
	Section *sec;

	for (sec = seclist; sec; sec = sec->next)
		isect(sec);
}

void
emit(char *bytes, int n)
{
	if (cursec->mem) {
		size_t len = cursec->pc - cursec->base;
		memcpy(&cursec->mem[len], bytes, n);
	}
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
	sym->section = NULL;

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

String
newstring(char *s)
{
	size_t len = strlen(s) + 1;
	String str;

	str.offset = 0;
	str.buf = xmalloc(len);
	memcpy(str.buf, s, len);
	return str;
}
