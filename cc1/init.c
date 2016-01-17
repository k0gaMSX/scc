
#include <stdio.h>
#include <stdlib.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"
#include "cc1.h"

typedef struct inititlizer Init;

struct designator {
	TINT pos;
	Node *expr;
	struct designator *next;
};

struct inititlizer {
	Type *type;
	TUINT curpos;
	struct designator *head;
};

static TINT
arydesig(Init *ip)
{
	TINT npos;
	Node *np;

	if (ip->type->op != ARY)
		errorp("array index in non-array initializer");
	next();
	np = iconstexpr();
	npos = np->sym->u.i;
	freetree(np);
	expect(']');
	return npos;
}

static TINT
fielddesig(Init *ip)
{
	TINT npos;
	int ons;
	Symbol *sym, **p;
	Type *tp = ip->type;

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

static Init *
designation(Init *ip)
{
	struct designator *dp;
	TINT (*fun)(Init *);

	dp = xmalloc(sizeof(*dp));
	dp->next = ip->head;
	ip->head = dp;

	switch (yytoken) {
	case '[': fun = arydesig;   break;
	case '.': fun = fielddesig; break;
	default:  return ip;
	}

	dp->pos  = (*fun)(ip);
	expect('=');
	return ip;
}

static Node *
initlist(Symbol *sym, Type *tp)
{
	struct inititlizer *ip;
	int toomany = 0;
	TINT n;
	Type *newtp;

	ip = xmalloc(sizeof(*ip));
	ip->head = NULL;
	if (accept('}'))
		return NULL;

	for (ip->curpos = 0; ; ++ip->curpos) {
		designation(ip);
		switch (tp->op) {
		case ARY:
			newtp = tp->type;
			if (!tp->defined || n < tp->n.elem)
				break;
			if (!toomany)
				warn("excess elements in array initializer");
			toomany = 1;
			sym = NULL;
			break;
		case STRUCT:
			if (n < tp->n.elem) {
				sym = tp->p.fields[n];
				newtp = sym->type;
				break;
			}
			if (!toomany)
				warn("excess elements in struct initializer");
			toomany = 1;
			sym = NULL;
			break;
		default:
			newtp = tp;
			warn("braces around scalar initializer");
			if (n <= 0)
				break;
			if (!toomany)
				warn("excess elements in scalar initializer");
			toomany = 1;
			sym = NULL;
			break;
		}
		if (accept('{'))
			return initlist(sym, tp);
		ip->head->expr = assign(NULL);

		if (!accept(','))
			break;
	}
	expect('}');

	if (tp->op == ARY && !tp->defined) {
		tp->n.elem = n + 1;
		tp->defined = 1;
	}
	return NULL;
}

void
initializer(Symbol *sym, Type *tp, int nelem)
{
	Node *np;
	int flags = sym->flags;

	if (tp->op == FTN)
		errorp("function '%s' is initialized like a variable", sym->name);

	switch (yytoken) {
	case '{':
		initlist(sym, tp);
		return;
	case '=':
		np = assign(varnode(sym));
		break;
	}

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
