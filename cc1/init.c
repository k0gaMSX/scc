
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"
#include "cc1.h"


typedef struct init Init;

struct designator {
	TINT pos;
	Node *expr;
	struct designator *next;
};

struct init {
	Type *type;
	size_t pos;
	size_t max;
	struct designator *tail;
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

	np = (accept('{')) ? initlist(tp) : decay(assign());
	if ((np = convert(np, tp, 0)) == NULL) {
		errorp("incorrect initializer");
		np = constnode(zero);
	}
	return np;
}

static Node *
mkcompound(Init *ip)
{
	Node **v, **p;
	size_t n;
	struct designator *dp, *next;
	Symbol *sym;

	n = ip->max;
	if (n >= n * sizeof(*v)) {
		errorp("compound literal too big");
		return constnode(zero);
	}
	n *= sizeof(*v);
	v = memset(xmalloc(n), 0, n);

	for (dp = ip->head; dp; dp = next) {
		p = &v[dp->pos];
		freetree(*p);
		*p = dp->expr;
		next = dp->next;
		free(dp);
	}

	sym = newsym(NS_IDEN);
	sym->u.init = v;
	sym->type = ip->type;
	sym->flags |= ISINITLST;

	return constnode(sym);
}

static void
newdesig(Init *ip, Node *np)
{
	struct designator *dp;

	if (ip->pos > ip->max)
		ip->max = ip->pos;

	dp = xmalloc(sizeof(*dp));
	dp->pos = ip->pos;
	dp->expr = np;
	dp->next = NULL;

	if (ip->head == NULL) {
		ip->head = ip->tail = dp;
	} else {
		ip->tail->next = dp;
		ip->tail = dp;
	}
}

static Node *
initlist(Type *tp)
{
	Init in;
	int toomany = 0, outbound;
	Type *newtp;
	Node *np;

	in.tail = in.head = NULL;
	in.type = tp;
	in.pos = 0;
	in.max = 0;

	do {
		if (yytoken == '}')
			break;
		outbound = 0;
		designation(&in);
		switch (tp->op) {
		case ARY:
			newtp = tp->type;
			if (!tp->defined || in.pos < tp->n.elem)
				break;
			if (!toomany)
				warn("excess elements in array initializer");
			outbound = 1;
			toomany = 1;
			break;
		/* TODO: case UNION: */
		case STRUCT:
			if (in.pos < tp->n.elem) {
				newtp = tp->p.fields[in.pos]->type;
				break;
			}
			newtp = inttype;
			if (!toomany)
				warn("excess elements in struct initializer");
			toomany = 1;
			outbound = 1;
			break;
		default:
			newtp = tp;
			warn("braces around scalar initializer");
			if (in.pos == 0)
				break;
			if (!toomany)
				warn("excess elements in scalar initializer");
			toomany = 1;
			outbound = 1;
			break;
		}

		np = initialize(newtp);
		if (outbound)
			freetree(np);
		else
			newdesig(&in, np);

		if (++in.pos == 0)
			errorp("compound literal too big");

	} while (accept(','));

	expect('}');

	if (tp->op == ARY && !tp->defined) {
		tp->n.elem = in.pos;
		tp->defined = 1;
	}
	if (tp->op == ARY || tp->op == STRUCT)
		in.max = tp->n.elem;
	else if (in.max == 0) {
		errorp("empty scalar initializer");
		return constnode(zero);
	}

	return mkcompound(&in);
}

void
initializer(Symbol *sym, Type *tp, int nelem)
{
	Node *np;
	int flags = sym->flags;

	if (tp->op == FTN)
		errorp("function '%s' is initialized like a variable", sym->name);
	np = initialize(tp);

	if (flags & ISDEFINED) {
		errorp("redeclaration of '%s'", sym->name);
	} else if ((flags & (ISGLOBAL|ISLOCAL|ISPRIVATE)) != 0) {
		if (!np->constant)
			errorp("initializer element is not constant");
		emit(OINIT, np);
		sym->flags |= ISDEFINED;
	} else if ((flags & (ISEXTERN|ISTYPEDEF)) != 0) {
		errorp("'%s' has both '%s' and initializer",
		       sym->name, (flags&ISEXTERN) ? "extern" : "typedef");
	} else {
		np = node(OASSIGN, tp, varnode(sym), np);
		emit(OEXPR, np);
	}
}
