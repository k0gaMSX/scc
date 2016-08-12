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

static char opasmw[] = {
	[OADD] = ASADDW,
	[OSUB] = ASSUBW,
	[OMUL] = ASMULW,
	[OMOD] = ASMODW,
	[ODIV] = ASDIVW,
	[OSHL] = ASSHLW,
	[OSHR] = ASSHRW,
	[OLT] = ASLTW,
	[OGT] = ASGTW,
	[OLE] = ASLEW,
	[OGE] = ASGEW,
	[OEQ] = ASEQW,
	[ONE] = ASNEW,
	[OBAND] = ASBANDW,
	[OBOR] = ASBORW,
	[OBXOR] = ASBXORW,
	[OCPL] = ASCPLW
};

static char opasml[] = {
	[OADD] = ASADDL,
	[OSUB] = ASSUBL,
	[OMUL] = ASMULL,
	[OMOD] = ASMODL,
	[ODIV] = ASDIVL,
	[OSHL] = ASSHLL,
	[OSHR] = ASSHRL,
	[OLT] = ASLTL,
	[OGT] = ASGTL,
	[OLE] = ASLEL,
	[OGE] = ASGEL,
	[OEQ] = ASEQL,
	[ONE] = ASNEL,
	[OBAND] = ASBANDL,
	[OBOR] = ASBORL,
	[OBXOR] = ASBXORL,
	[OCPL] = ASCPLL
};

static char opasms[] = {
	[OADD] = ASADDS,
	[OSUB] = ASSUBS,
	[OMUL] = ASMULS,
	[OMOD] = ASMODS,
	[ODIV] = ASDIVS,
	[OSHL] = ASSHLS,
	[OSHR] = ASSHRS,
	[OLT] = ASLTS,
	[OGT] = ASGTS,
	[OLE] = ASLES,
	[OGE] = ASGES,
	[OEQ] = ASEQS,
	[ONE] = ASNES,
	[OBAND] = ASBANDS,
	[OBOR] = ASBORS,
	[OBXOR] = ASBXORS,
	[OCPL] = ASCPLS
};
static char opasmd[] = {
	[OADD] = ASADDD,
	[OSUB] = ASSUBD,
	[OMUL] = ASMULD,
	[OMOD] = ASMODD,
	[ODIV] = ASDIVD,
	[OSHL] = ASSHLD,
	[OSHR] = ASSHRD,
	[OLT] = ASLTD,
	[OGT] = ASGTD,
	[OLE] = ASLED,
	[OGE] = ASGED,
	[OEQ] = ASEQD,
	[ONE] = ASNED,
	[OBAND] = ASBANDD,
	[OBOR] = ASBORD,
	[OBXOR] = ASBXORD,
	[OCPL] = ASCPLD
};

extern Type int32type;

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
assign(Node *to, Node *from)
{
	Type *tp;
	int op;

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

static void
bool(Node *np, Symbol *true, Symbol *false)
{
	Node *l = np->left, *r = np->right;
	Node ret, *ifyes, *ifno;
	Symbol *label;

	switch (np->op) {
	case OAND:
		label = newlabel();
		bool(l, label, true);
		setlabel(label);
		bool(r, true, false);
		break;
	case OOR:
		label = newlabel();
		bool(l, true, label);
		setlabel(label);
		bool(r, true, false);
		break;
	default:
		ifyes = label2node(true);
		ifno = label2node(false);
		rhs(l, &ret);
		code(ASBRANCH, &ret, ifyes, ifno);
		deltree(ifyes);
		deltree(ifno);
		break;
	}
}

static Node *
function(void)
{
	Symbol *p;

	/* allocate stack space for parameters */
	for (p = locals; p && (p->type.flags & PARF) != 0; p = p->next)
		code(ASALLOC, label2node(p), NULL, NULL);

	/* allocate stack space for local variables) */
	for ( ; p && p->id != TMPSYM; p = p->next) {
		if (p->kind != SAUTO)
			continue;
		code(ASALLOC, label2node(p), NULL, NULL);
	}
	/* store formal parameters in parameters */
	for (p = locals; p; p = p->next) {
		if ((p->type.flags & PARF) == 0)
			break;
		code(ASFORM, label2node(p), NULL, NULL);
	}
	return NULL;
}

static Node *
rhs(Node *np, Node *ret)
{
	Node aux1, aux2, *phi, *l = np->left, *r = np->right;
	Type *tp;
	int off, op;
	char *tbl;
	Symbol *true, *false;

	tp = &np->type;

	switch (np->op) {
	case OBFUN:
		return function();
	case ONOP:
	case OEFUN:
		return NULL;
	case OCONST:
	case OMEM:
	case OAUTO:
		return load(np, ret);
	case OAND:
	case OOR:
		true = newlabel();
		false = newlabel();
		phi = label2node(newlabel());
		ret->type = int32type;
		tmpnode(ret);

		bool(np, true, false);

		setlabel(true);
		assign(ret, constnode(1, &int32type));
		code(ASJMP, NULL, phi, NULL);

		setlabel(false);
		assign(ret, constnode(0, &int32type));

		setlabel(phi->u.sym);
		deltree(phi);
		return ret;
        case OSHR:
        case OMOD:
        case ODIV:
        case OLT:
        case OGT:
        case OLE:
        case OGE:
                /*
                 * unsigned version of operations are always +1 the
                 * signed version
                 */
                off = (tp->flags & SIGNF) == 0;
                goto binary;
        case OADD:
        case OSUB:
        case OMUL:
        case OSHL:
        case OBAND:
        case OBOR:
        case OBXOR:
        case OEQ:
        case ONE:
                off = 0;
        binary:
		if (l->complex >= r->complex) {
			rhs(l, &aux1);
			rhs(r, &aux2);
		} else {
			rhs(r, &aux2);
			rhs(l, &aux1);
		}
                switch (tp->size) {
                case 4:
                        tbl = (tp->flags & INTF) ? opasmw : opasms;
                        break;
                case 8:
                        tbl = (tp->flags & INTF) ? opasml : opasmd;
                        break;
                default:
                        abort();
                }
                op = tbl[np->op] + off;
		ret = tmpnode(ret);
                code(op, ret, &aux1, &aux2);
                return ret;
	case OASSIG:
		lhs(l, &aux1);
		rhs(r, ret);
		return assign(&aux1, ret);
	default:
		abort();
	}
	abort();
}

Node *
cgen(Node *np)
{
	Node ret, *aux, *next, *phi, *ifyes, *ifno;

	setlabel(np->label);
	switch (np->op) {
	case OJMP:
		ifyes = label2node(np->u.sym);
		code(ASJMP, NULL, ifyes, NULL);
		deltree(ifyes);
		break;
        case OBRANCH:
                next = np->next;
                if (!next->label)
                        next->label = newlabel();

                ifyes = label2node(np->u.sym);
                ifno = label2node(next->label);
		rhs(np->left, &ret);
		code(ASBRANCH, &ret, ifyes, ifno);
                deltree(ifyes);
                deltree(ifno);
                break;
	case ORET:
		aux = (np->left) ? rhs(np->left, &ret) : NULL;
		code(ASRET, NULL, aux, NULL);
		break;
	default:
		rhs(np, &ret);
		break;
	}
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
		rp = constnode(~(TUINT) 0, &np->type);
		goto binary;
	case ONEG:
		np->op = OSUB;
		rp = lp;
		lp = constnode(0, &np->type);
		if ((np->type.flags & INTF) == 0)
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
