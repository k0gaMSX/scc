
#include <stdio.h>
#include <stdlib.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"
#include "cc1.h"


struct designator {
	TINT pos;
	Node *expr;
	struct designator *next;
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

	ip->pos  = (*fun)(ip);
	expect('=');
	return ip;
}

static Node *initlist(Type *tp);

static Node *
initialize(Type *tp)
{
	Node *np;

	np = (accept('{')) ? initlist(tp) : assign();
	if ((np = convert(decay(np), tp, 0)) == NULL)
		errorp("incorrect initializer");
	return np;
}

static Node *
initlist(Type *tp)
{
	Init *ip;
	Symbol *sym;
	struct designator *dp;
	int toomany = 0;
	Type *newtp;

	ip = xmalloc(sizeof(*ip));
	ip->head = NULL;
	ip->pos = 0;
	ip->type = tp;

	if (accept('}'))
		goto end_of_initializer;

	for (ip->pos = 1; ; ++ip->pos) {
		designation(ip);
		switch (tp->op) {
		case ARY:
			newtp = sym->type;
			if (!tp->defined || ip->pos <= tp->n.elem)
				break;
			if (!toomany)
				warn("excess elements in array initializer");
			toomany = 1;
			break;
		case STRUCT:
			if (ip->pos <= tp->n.elem) {
				sym = tp->p.fields[ip->pos-1];
				newtp = sym->type;
				break;
			}
			newtp = inttype;
			if (!toomany)
				warn("excess elements in struct initializer");
			toomany = 1;
			break;
		default:
			newtp = tp;
			warn("braces around scalar initializer");
			if (ip->pos == 1)
				break;
			if (!toomany)
				warn("excess elements in scalar initializer");
			toomany = 1;
			break;
		}
		dp = ip->head;
		dp->pos = ip->pos;
		dp->expr = initialize(newtp);

		if (!accept(','))
			break;
	}
	expect('}');

end_of_initializer:
	if (tp->op == ARY && !tp->defined) {
		tp->n.elem = ip->pos;
		tp->defined = 1;
	}
	sym = newsym(NS_IDEN);
	sym->u.init = ip;
	sym->type = tp;
	return constnode(sym);
}

void
initializer(Symbol *sym, Type *tp, int nelem)
{
	Node *np;
	int flags = sym->flags;

	if (tp->op == FTN)
		errorp("function '%s' is initialized like a variable", sym->name);

	np = node(OINIT, tp, varnode(sym), initialize(tp));

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
