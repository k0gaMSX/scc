/* See LICENSE file for copyright and license details. */
static char sccsid[] = "@(#) ./cc1/symbol.c";
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstd.h>
#include "../inc/cc.h"
#include "cc1.h"

#define NR_SYM_HASH 64

unsigned curctx;
static unsigned short counterid;

static Symbol *head, *labels;
static Symbol *htab[NR_SYM_HASH];
static Symbol *htabcpp[NR_SYM_HASH];

#ifndef NDEBUG
void
dumpstab(char *msg)
{
	Symbol **bp, *sym;

	fprintf(stderr, "Symbol Table dump at ctx=%u\n%s\n", curctx, msg);
	for (bp = htab; bp < &htab[NR_SYM_HASH]; ++bp) {
		if (*bp == NULL)
			continue;
		fprintf(stderr, "%d", (int) (bp - htab));
		for (sym = *bp; sym; sym = sym->hash)
			fprintf(stderr, "->[%d,%d:'%s'=%p]",
			        sym->ns, sym->ctx, sym->name, (void *) sym);
		putc('\n', stderr);
	}
	fputs("head:", stderr);
	for (sym = head; sym; sym = sym->next) {
		fprintf(stderr, "->[%d,%d:'%s'=%p]",
		        sym->ns, sym->ctx,
		        (sym->name) ? sym->name : "", (void *) sym);
	}
	putc('\n', stderr);
}
#endif

static Symbol **
hash(const char *s, int ns)
{
	unsigned c, h;
	Symbol **tab;


	for (h = 0; c = *s; ++s)
		h ^= 33 * c;
	h &= NR_SYM_HASH-1;

	tab = (ns == NS_CPP) ? htabcpp : htab;
	return &tab[h];
}

static void
unlinkhash(Symbol *sym)
{
	Symbol **h;

	if ((sym->flags & SDECLARED) == 0)
		return;
	h = hash(sym->name, sym->ns);
	*h = sym->hash;
}

void
pushctx(void)
{
	if (++curctx == NR_BLOCK+1)
		error("too many nested blocks");
}

void
killsym(Symbol *sym)
{
	short f;
	char *name;

	if (!sym)
		return;
	f = sym->flags;
	if (f & SSTRING)
		free(sym->u.s);
	if (sym->ns == NS_TAG)
		sym->type->prop &= ~TDEFINED;
	unlinkhash(sym);
	if ((name = sym->name) != NULL && sym->ns != NS_CPP) {
		if ((f & (SUSED|SGLOBAL|SDECLARED)) == SDECLARED)
			warn("'%s' defined but not used", name);
		if ((f & SDEFINED) == 0 && sym->ns == NS_LABEL)
			errorp("label '%s' is not defined", name);
	}
	free(name);
	free(sym);
}

void
popctx(void)
{
	Symbol *next, *sym;

	for (sym = head; sym && sym->ctx == curctx; sym = next) {
		next = sym->next;
		killsym(sym);
	}
	head = sym;

	if (--curctx != GLOBALCTX)
		return;

	for (sym = labels; sym; sym = next) {
		next = sym->next;
		killsym(sym);
	}
	labels = NULL;

	if (curfun) {
		free(curfun->u.pars);
		curfun->u.pars = NULL;
	}
}

unsigned
newid(void)
{
	unsigned short id;

	if (lexmode == CPPMODE)
		return 0;
	id = ++counterid;
	if (id == 0) {
		die("Overflow in %s identifiers",
		    (curctx) ? "internal" : "external");
	}
	return id;
}

Symbol *
newsym(int ns, char *name)
{
	Symbol *sym;

	sym = xmalloc(sizeof(*sym));
	if (name)
		name = xstrdup(name);
	sym->name = name;
	sym->id = 0;
	sym->ns = ns;
	sym->ctx = (ns == NS_CPP) ? UCHAR_MAX : curctx;
	sym->token = IDEN;
	sym->flags = 0;
	sym->u.s = NULL;
	sym->type = NULL;
	sym->hash = NULL;

	if (ns == NS_LABEL) {
		sym->next = labels;
		labels = sym;
	} else if (ns != NS_CPP) {
		sym->next = head;
		head = sym;
	}
	return sym;
}

static Symbol *
linkhash(Symbol *sym)
{
	Symbol **h;

	h = hash(sym->name, sym->ns);
	sym->hash = *h;
	*h = sym;

	if (sym->ns != NS_CPP)
		sym->id = newid();
	sym->flags |= SDECLARED;
	return sym;
}

Symbol *
newstring(char *s, size_t len)
{
	Symbol *sym = newsym(NS_IDEN, NULL);

	if (lexmode != CPPMODE)
		sym->type = mktype(chartype, ARY, len, NULL);
	sym->id = newid();
	sym->flags |= SSTRING | SCONSTANT | SPRIVATE;
	sym->u.s = xmalloc(len);
	if (s)
		memcpy(sym->u.s, s, len);

	return sym;
}

Symbol *
newlabel(void)
{
	Symbol *sym = newsym(NS_LABEL, NULL);
	sym->id = newid();
	return sym;
}

Symbol *
lookup(int ns, char *name, int alloc)
{
	Symbol *sym;
	int sns;
	char *t, c;

	c = *name;
	for (sym = *hash(name, ns); sym; sym = sym->hash) {
		t = sym->name;
		if (*t != c || strcmp(t, name))
			continue;
		sns = sym->ns;
		if (sns == ns)
			return sym;
		/*
		 * When a lookup is done in a namespace associated
		 * to a struct we also want symbols of NS_IDEN which
		 * are typedef, because in other case we cannot declare
		 * fields of such types.
		 * TODO: Remove this trick
		 */
		if (sns == NS_KEYWORD ||
		    (sym->flags & STYPEDEF) && ns >= NS_STRUCTS) {
			return sym;
		}
	}
	return (alloc == ALLOC) ? newsym(ns, name) : NULL;
}

Symbol *
install(int ns, Symbol *sym)
{
	if (sym->flags & SDECLARED) {
		if (sym->ctx == curctx && ns == sym->ns)
			return NULL;
		sym = newsym(ns, sym->name);
	}
	return linkhash(sym);
}

void
keywords(struct keyword *key, int ns)
{
	Symbol *sym;

	for ( ; key->str; ++key) {
		sym = linkhash(newsym(ns, key->str));
		sym->token = key->token;
		sym->u.token = key->value;
	}
	/*
	 * Remove all the predefined symbols from * the symbol list. It
	 * will make faster some operations. There is no problem of memory
	 * leakeage because this memory is not ever freed
	 */
	counterid = 0;
	head = NULL;
}
