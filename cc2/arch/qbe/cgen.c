/* See LICENSE file for copyright and license details. */
#include <assert.h>
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"
#include "../../../inc/sizes.h"

enum sflags {
	ISTMP  = 1,
	ISCONS = 2
};

static Node *
tmpnode(Node *np)
{
	Symbol *sym;

	sym = getsym(TMPSYM);
	sym->type = np->type;
	sym->kind = STMP;
	np->u.sym = sym;
	np->op = OTMP;
	np->flags |= ISTMP;
	return np;
}

static Node *
load(Node *np, Node *new)
{
	int op;
	Type *tp;

	tp = &np->type;
	new->type = *tp;
	tmpnode(new);

	switch (tp->size) {
	case 1:
		op = ASLDB;
		break;
	case 2:
		op = ASLDH;
		break;
	case 4:
		op = (tp->flags & INTF) ? ASLDW : ASLDS;
		break;
	case 8:
		op = (tp->flags & INTF) ? ASLDL : ASLDD;
		break;
	default:
		abort();
	}
	code(op, new, np, NULL);
	return new;
}

static Node *
assign(Node *np, Node *new)
{
	Node *to, *from;
	Type *tp;
	int op;

	to = np->left;
	from = np->right;

	tp = &to->type;
	switch (tp->size) {
	case 1:
		op = ASSTB;
		break;
	case 2:
		op = ASSTH;
		break;
	case 4:
		op = (tp->flags & INTF) ? ASSTW : ASSTS;
		break;
	case 8:
		op = (tp->flags & INTF) ? ASSTL : ASSTD;
		break;
	default:
		abort();
	}
	code(op, to, from, NULL);
	return from;
}

static Node *rhs(Node *np, Node *new);

static Node *
lhs(Node *np, Node *new)
{
	switch (np->op) {
	case OMEM:
	case OAUTO:
		return np;
	case OPTR:
		return rhs(np->left, new);
	default:
		abort();
	}
}

static Node *
bool(Node *np, Node *new, Symbol *true, Symbol *false)
{
	Node *l = np->left, *r = np->right;
	Symbol *label;

	switch (np->op) {
	case OAND:
		label = newlabel();
		bool(l, new, label, true);
		setlabel(label);
		bool(r, new, true, false);
		break;
	case OOR:
		label = newlabel();
		bool(l, new, true, label);
		setlabel(label);
		bool(r, new, true, false);
		break;
	default:
		rhs(np, new);
		code(ASBRANCH, new, label2node(true), label2node(false));
		break;
	}
	return new;
}

static Node *
rhs(Node *np, Node *new)
{
	Node aux;
	Symbol *label1, *label2;

	switch (np->op) {
	case OMEM:
	case OAUTO:
		return load(np, new);
	case OJMP:
	case OBRANCH:
	case ORET:
		label1 = newlabel();
		label2 = newlabel();
		bool(np, new, label1, label2);
		setlabel(label1);
		return NULL;
	case OASSIG:
		lhs(np->left, new);
		rhs(np->right, &aux);
		assign(&aux, new);
		return new;
	default:
		abort();
	}
}

Node *
cgen(Node *np)
{
	Node n;

	rhs(np, &n);
	return NULL;
}

/*
 * This is strongly influenced by
 * http://plan9.bell-labs.com/sys/doc/compiler.ps (/sys/doc/compiler.ps)
 * calculate addresability as follows
 *     AUTO => 11          value+fp
 *     REG => 11           reg
 *     STATIC => 11        (value)
 *     CONST => 11         $value
 * These values of addressability are not used in the code generation.
 * They are only used to calculate the Sethi-Ullman numbers. Since
 * QBE is AMD64 targered we could do a better job there, and try to
 * detect some of the complex addressing modes of these processors.
 */
Node *
sethi(Node *np)
{
	Node *lp, *rp;

	if (!np)
		return np;

	np->complex = 0;
	np->address = 0;
	lp = np->left;
	rp = np->right;

	switch (np->op) {
	case OAUTO:
	case OREG:
	case OMEM:
	case OCONST:
		np->address = 11;
		break;
	case OCPL:
		np->op = OAND;
		rp = newnode(OCONST);
		rp->type = np->type;
		rp->u.i = 0;
		rp->u.i = ~np->u.i;
		goto binary;
	case ONEG:
		np->op = OSUB;
		rp = lp;
		lp = newnode(OCONST);
		lp->type = np->type;
		if (np->type.flags & INTF)
			lp->u.i = 0;
		else
			lp->u.f = 0.0;
	default:
	binary:
		lp = sethi(lp);
		rp = sethi(rp);
		break;
	}
	np->left = lp;
	np->right = rp;

	if (np->address > 10)
		return np;
	if (lp)
		np->complex = lp->complex;
	if (rp) {
		int d = np->complex - rp->complex;

		if (d == 0)
			++np->complex;
		else if (d < 0)
			np->complex = rp->complex;
	}
	if (np->complex == 0)
		++np->complex;
	return np;
}
