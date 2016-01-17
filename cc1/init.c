
#include <stdio.h>
#include <stdlib.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"
#include "cc1.h"

struct designator {
	TINT pos;
	struct designator *next;
};

static TINT
arydesig(Type *tp)
{
	TINT npos;
	Node *np;

	if (tp->op != ARY)
		errorp("array index in non-array initializer");
	next();
	np = iconstexpr();
	npos = np->sym->u.i;
	freetree(np);
	expect(']');
	return npos;
}

static TINT
fielddesig(Type *tp)
{
	TINT npos;
	int ons;
	Symbol *sym, **p;

	if (!tp->aggreg)
		errorp("field name not in record or union initializer");
	ons = namespace;
	namespace = tp->ns;
	next();
	namespace = ons;
	if (yytoken != IDEN)
		unexpected();
	sym = yylval.sym;
	if ((sym->flags & ISDECLARED) == 0) {
		errorp(" unknown field '%s' specified in initializer",
		      sym->name);
		return 0;
	}
	for (p = tp->p.fields; *p != sym; ++p)
		/* nothing */;
	return p - tp->p.fields;
}

static struct designator *
designation(Type *tp)
{
	struct designator *des = NULL, *d;
	TINT (*fun)(Type *);

	for (;;) {
		switch (yytoken) {
		case '[': fun = arydesig;   break;
		case '.': fun = fielddesig; break;
		default:
			if (des)
				expect('=');
			return des;
		}
		d = xmalloc(sizeof(*d));
		d->next = NULL;

		if (!des) {
			des = d;
		} else {
			des->next = d;
			des = d;
		}
		des->pos  = (*fun)(tp);
	}
}

static void
initlist(Symbol *sym, Type *tp)
{
	struct designator *des;
	int toomany = 0;
	TINT n;
	Type *newtp;

	for (n = 0; ; ++n) {
		if ((des = designation(tp)) == NULL) {
			des = xmalloc(sizeof(*des));
			des->pos = n;
		} else {
			n = des->pos;
		}
		switch (tp->op) {
		case ARY:
			if (tp->defined && n >= tp->n.elem) {
				if (!toomany)
					warn("excess elements in array initializer");
				toomany = 1;
				sym = NULL;
			}
			newtp = tp->type;
			break;
		case STRUCT:
			if (n >= tp->n.elem) {
				if (!toomany)
					warn("excess elements in struct initializer");
				toomany = 1;
				sym = NULL;
			} else {
				sym = tp->p.fields[n];
				newtp = sym->type;
			}
			break;
		default:
			newtp = tp;
			warn("braces around scalar initializer");
			if (n > 0) {
				if (!toomany)
					warn("excess elements in scalar initializer");
				toomany = 1;
				sym = NULL;
			}
			break;
		}
		initializer(sym, newtp, n);
		if (!accept(','))
			break;
	}
	expect('}');

	if (tp->op == ARY && !tp->defined) {
		tp->n.elem = n + 1;
		tp->defined = 1;
	}
}

extern Node *assign(Node *np);

void
initializer(Symbol *sym, Type *tp, int nelem)
{
	Node *np;
	int flags = sym->flags;

	if (tp->op == FTN)
		errorp("function '%s' is initialized like a variable", sym->name);

	switch (yytoken) {
	case '{':
		initlist(sym, tp); /* FIXME: This code is not complete */
		return;
	case '=':
		np = assign(varnode(sym));
		break;
	}

	/* FIXME: old code used in the recursive call
	 * if (!sym)
	 *	return;
	 * if (nelem >= 0)
	 *	return;
	 */

	if (flags & ISDEFINED) {
		errorp("redeclaration of '%s'", sym->name);
	} else if ((flags & (ISGLOBAL|ISLOCAL|ISPRIVATE)) != 0) {
		if (!np->right->constant)
			errorp("initializer element is not constant");
		emit(OINIT, np);
		sym->flags |= ISDEFINED;
	} else if ((flags & (ISEXTERN|ISTYPEDEF)) != 0) {
		errorp("'%s' has both '%s' and initializer",
		       sym->name, (flags&ISEXTERN) ? "extern" : "typedef");
	} else {
		np->op = OASSIGN;
		emit(OEXPR, np);
	}
}
