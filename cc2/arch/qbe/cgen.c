
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"

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
load(Node *np)
{
	Node *new;

	new = tmpnode(newnode());
	new->left = np;
	new->type = np->type;
	code(ASLOAD, new, np, NULL);

	return new;
}

Node *
cgen(Node *np)
{
	Node *l, *r;
	Symbol *sym;
	Type *tp;
	int op, off;
	char *tbl;

	if (!np)
		return NULL;

	l = cgen(np->left);
	r = cgen(np->right);
	tp = &np->type;

	switch (np->op) {
	case OREG:
	case OSTRING:
		abort();
	case OCONST:
	case OLABEL:
		np->flags |= ISCONS;
	case OMEM:
	case OAUTO:
		return np;
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
	case OCPL:
	case OEQ:
	case ONE:
		off = 0;
	binary:
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
		code(ASASSIG, l, r, NULL);
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
		++np->complex;
	return np;
}
