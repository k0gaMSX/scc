
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"

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
	sym->kind = TMP;
	np->u.sym = sym;
	np->op = OTMP;
	np->flags |= ISTMP;
	return np;
}

static Node *
load(Node *np)
{
	Node *new;

	new = tmpnode(newnode());
	new->left = np;
	new->type = np->type;
	code(OLOAD, new, np, NULL);

	return new;
}

Node *
cgen(Node *np)
{
	Node *l, *r;
	Symbol *sym;
	int op;

	if (!np)
		return NULL;

	l = cgen(np->left);
	r = cgen(np->right);

	switch (op = np->op) {
	case OREG:
	case OSTRING:
		abort();
	case OCONST:
	case OLABEL:
		np->flags |= ISCONS;
	case OMEM:
	case OAUTO:
		return np;
	case OADD:
	case OSUB:
	case OMUL:
	case OMOD:
	case ODIV:
	case OSHL:
	case OSHR:
	case OLT:
	case OGT:
	case OLE:
	case OGE:
	case OEQ:
	case ONE:
	case OBAND:
	case OBOR:
	case OBXOR:
	case OCPL:
		if ((l->flags & (ISTMP|ISCONS)) == 0)
			l = np->left = load(l);
		if ((r->flags & (ISTMP|ISCONS)) == 0)
			r = np->right = load(r);
		tmpnode(np);
		code(op, np, l, r);
		return np;
	case ONOP:
	case OBLOOP:
	case OELOOP:
		return NULL;
	case OPAR:
	case ONEG:
	case OADDR:
	case OPTR:
	case OCAST:
	case OINC:
	case ODEC:
		abort();
	case OASSIG:
		code(op, l, l, r);
		return r;
	case OCALL:
	case OFIELD:
	case OCOMMA:
	case OASK:
	case OCOLON:
	case OAND:
	case OOR:
		abort();
	case OBRANCH:
	case OJMP:
		code(op, NULL, l, r);
		return NULL;
	case ORET:
	case OCASE:
	case ODEFAULT:
	case OTABLE:
	case OSWITCH:
	default:
		abort();
	}
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
 * QBE is AMD64 targered we could do a better job here, and try to
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
	default:
		sethi(lp);
		sethi(rp);
		break;
	}

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
		++rp->complex;
	return np;
}
