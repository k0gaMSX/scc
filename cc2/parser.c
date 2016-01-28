
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"

#include "arch.h"
#include "cc2.h"

#define MAXLINE     200
#define STACKSIZ     50

extern Type int8type, int16type, int32type, int64type,
            uint8type, uint16type, uint32type, uint64type,
            float32type, float64type, float80type,
            booltype,
            ptrtype,
            voidtype,
            elipsistype;

Type funtype = {
	.flags = FUNF
};

union tokenop {
	void *arg;
	int op;
};

typedef void parsefun(char *, union tokenop);
static parsefun type, symbol, getname, unary, binary, ternary, call,
                parameter, constant, composed, begininit, endinit,
                jump, oreturn, loop, assign, ocase, odefault, casetable;

typedef void evalfun(void);
static evalfun vardecl, beginfun, endfun, endpars, stmt,
               array, aggregate, flddecl, labeldcl;

static struct decoc {
	void (*eval)(void);
	void (*parse)(char *token, union tokenop);
	union tokenop u;
} optbl[] = {
	[AUTO]        = {vardecl, symbol, .u.op = AUTO},
	[REG]         = {vardecl, symbol, .u.op = REG},
	[GLOB]        = {vardecl, symbol, .u.op = MEM},
	[EXTRN]       = {vardecl, symbol, .u.op = MEM},
	[PRIVAT]      = {vardecl, symbol, .u.op = MEM},
	[LOCAL]       = {vardecl, symbol, .u.op = MEM},
	[MEMBER]      = {flddecl, symbol},
	[LABEL]       = {labeldcl, symbol},

	[INT8]        = {NULL, type, .u.arg   = &int8type},
	[INT16]       = {NULL, type, .u.arg   = &int16type},
	[INT32]       = {NULL, type, .u.arg   = &int32type},
	[INT64]       = {NULL, type, .u.arg   = &int64type},
	[UINT8]       = {NULL, type, .u.arg   = &uint8type},
	[UINT16]      = {NULL, type, .u.arg   = &uint16type},
	[UINT32]      = {NULL, type, .u.arg   = &uint32type},
	[UINT64]      = {NULL, type, .u.arg   = &uint64type},
	[FLOAT]       = {NULL, type, .u.arg   = &float32type},
	[DOUBLE]      = {NULL, type, .u.arg   = &float64type},
	[LDOUBLE]     = {NULL, type, .u.arg   = &float80type},
	[VOID]        = {NULL, type, .u.arg   = &voidtype},
	[BOOL]        = {NULL, type, .u.arg   = &booltype},
	[POINTER]     = {NULL, type, .u.arg   = &ptrtype},
	[ELLIPSIS]    = {NULL, type, .u.arg   = &elipsistype},

	[FUNCTION]    = {NULL, type, .u.arg = &funtype},
	[VECTOR]      = {array, composed},
	[UNION]       = {aggregate, composed},
	[STRUCT]      = {aggregate, composed},

	[ONAME]       = {NULL, getname},
	['{']         = {beginfun},
	['}']         = {endfun},
	['(']         = {NULL, begininit},
	[')']         = {NULL, endinit},
	[OEPARS]      = {endpars},
	[OSTMT]       = {stmt},

	[OCPL]        = {NULL, unary},
	[ONEG]        = {NULL, unary},
	[OADDR]       = {NULL, unary},
	[OPTR]        = {NULL, unary},
	[OCAST]       = {NULL, unary},
	[OPAR ]       = {NULL, unary},

	[OAND]        = {NULL, binary},
	[OOR]         = {NULL, binary},
	[OFIELD]      = {NULL, binary},
	[OADD]        = {NULL, binary},
	[OSUB]        = {NULL, binary},
	[OMUL]        = {NULL, binary},
	[OMOD]        = {NULL, binary},
	[ODIV]        = {NULL, binary},
	[OSHL]        = {NULL, binary},
	[OSHR]        = {NULL, binary},
	[OLT]         = {NULL, binary},
	[OGT]         = {NULL, binary},
	[OLE]         = {NULL, binary},
	[OGE]         = {NULL, binary},
	[OEQ]         = {NULL, binary},
	[ONE]         = {NULL, binary},
	[OBAND]       = {NULL, binary},
	[OBOR]        = {NULL, binary},
	[OBXOR]       = {NULL, binary},
	[OCOMMA]      = {NULL, binary},

	[OASSIG]      = {NULL, assign},
	[OASK]        = {NULL, ternary},
	[OCALL]       = {NULL, call},

	[OCONST]      = NULL, constant,

	[OJMP]        = NULL, jump,
	[OBRANCH]     = NULL, jump,
	[ORET]        = NULL, oreturn,

	[OBLOOP]      = NULL, loop,
	[OELOOP]      = NULL, loop,

	[OCASE]       = NULL, jump,
	[OSWITCH]     = NULL, jump,

	[ODEFAULT]    = NULL, odefault,
	[OTABLE]      = NULL, casetable,
};

static Symbol *curfun;
static int funpars = -1, sclass, ininit, endf, lineno;
static Node *stmtp;
static void *stack[STACKSIZ], **sp = stack;
static Symbol *params[NR_FUNPARAM];

static void
push(void *elem)
{
	if (sp == stack[STACKSIZ])
		error(ESTACKO);
	*sp++ = elem;
}

static void *
pop(void)
{
	if (sp == stack)
		error(ESTACKU);
	return *--sp;
}

static int
empty(void)
{
	return sp == stack;
}

static void
type(char *token, union tokenop u)
{
	push(u.arg);
}

static void
composed(char *token, union tokenop u)
{
	Symbol *sym;

	sym = getsym(atoi(token+1));
	push(&sym->type);
}

static void
getname(char *t, union tokenop u)
{
	push((*++t) ? xstrdup(t) : NULL);
}

static void
symbol(char *token, union tokenop u)
{
	Node *np;

	sclass = *token++;
	np = newnode();
	np->u.sym = getsym(atoi(token));
	np->op = u.op;
	push(np);
}

static Type *
gettype(char *token)
{
	struct decoc *dp;

	dp = &optbl[*token];
	if (!dp->parse)
		error(ESYNTAX);
	(*dp->parse)(token, dp->u);
	return pop();
}

static void
constant(char *token, union tokenop u)
{
	static char letters[] = "0123456789ABCDEF";
	Node *np = newnode();
	TUINT v;
	unsigned c;

	++token;
	if (*token == OSTRING) {
		++token;
		np->op = OSTRING;
		np->type.flags = STRF;
		np->type.size = strlen(token);
		np->type.align = int8type.align;
		np->u.s = xstrdup(token);
	} else {
		np->op = OCONST;
		np->type = *gettype(token++);
		for (v = 0; c = *token++; v += c) {
			v <<= 4;
			c = strchr(letters, c) - letters;
		}
		np->u.i = v;
	}
	push(np);
}

static void
assign(char *token, union tokenop u)
{
	int c, op = *token++;
	Node *np = newnode();

	switch (*token) {
	case ODIV:
	case OMOD:
	case OADD:
	case OSUB:
	case OSHL:
	case OSHR:
	case OBAND:
	case OBXOR:
	case OBOR:
	case OINC:
	case ODEC:
		c = *token++;
		break;
	default:
		c = 0;
		break;
	}

	np->u.subop = c;
	np->op = op;
	np->type = *gettype(token);
	np->right = pop();
	np->left = pop();
	push(np);
}

static void
ternary(char *token, union tokenop u)
{
	Node *ask, *colon;
	Type *tp;

	tp = gettype(++token);

	colon = newnode();
	colon->op = OCOLON;
	colon->right = pop();
	colon->left = pop();

	ask = newnode();
	ask->op = OASK;
	ask->type = *tp;
	ask->left = pop();
	push(ask);
}

static Node *
eval(char *tok)
{
	struct decoc *dp;

	do {
		dp = &optbl[*tok];
		if (!dp->parse)
			break;
		(*dp->parse)(tok, dp->u);
	} while (tok = strtok(NULL, "\t\n"));
}

static int
nextline(void)
{
	char line[MAXLINE];
	size_t len;
	int c;
	void (*fun)(void);

repeat:
	++lineno;
	if (!fgets(line, sizeof(line), stdin))
		return 0;
	if ((len = strlen(line)) == 0 || line[0] == '\n')
		goto repeat;
	if (line[len-1] != '\n')
		error(ELNLINE);
	line[len-1] = '\0';

	c = *line;
	eval(strtok(line, "\t\n"));
	if ((fun = *optbl[c].eval) != NULL)
		(*fun)();
	if (sp != stack)
		error(ESTACKA);
	return 1;
}

static void
oreturn(char *token, union tokenop u)
{
	Node *np;

	np = newnode();
	np->op = *token;
	eval(strtok(NULL, "\t\n"));
	if (!empty())
		np->left = pop();
	push(np);
}

static void
jump(char *token, union tokenop u)
{
	Node *np, *aux;

	np = newnode();
	np->op = *token;
	eval(strtok(NULL, "\t\n"));

	if (*token != OJMP)
		np->left = pop();
	aux = pop();
	np->u.sym = aux->u.sym;
	delnode(aux);
	push(np);
}

static void
casetable(char *token, union tokenop u)
{
	Node *np, *aux;

	np = newnode();
	np->op = *token;
	eval(strtok(NULL, "\t\n"));
	np->left = pop();
	push(np);
}

static void
odefault(char *token, union tokenop u)
{
	Node *np;

	np = newnode();
	np->op = *token;
	eval(strtok(NULL, "\t\n"));
	np->left = pop();
	push(np);
}

static void
loop(char *token, union tokenop u)
{
	Node *np;

	np = newnode();
	np->op = *token;
	push(np);
}

static void
unary(char *token, union tokenop u)
{
	Node *np = newnode();

	np->op = *token++;
	np->type = *gettype(token);
	np->left = pop();
	np->right = NULL;
	push(np);
}

static void
call(char *token, union tokenop u)
{
	Node *np, *par, *fun;

	for (par = NULL;; par = np) {
		np = pop();
		if (np->op != OPAR)
			break;
		np->right = par;
	}
	fun = newnode();
	fun->op = *token++;
	fun->type = *gettype(token);
	fun->left = np;
	fun->right = par;
	push(fun);
}

static void
binary(char *token, union tokenop u)
{
	Node *np = newnode();

	np->op = *token++;
	np->type = *gettype(token);
	np->right = pop();
	np->left = pop();
	push(np);
}

static void
begininit(char *token, union tokenop u)
{
	ininit = 1;
}

static void
endinit(char *token, union tokenop u)
{
	ininit = 0;
}

static void
endpars(void)
{
	if (!curfun || funpars == -1)
		error(ESYNTAX);
	funpars = -1;
}

static void
aggregate(void)
{
	Node *align, *size;
	char *name;
	Type *tp;
	Symbol *sym;

	align = pop();
	size = pop();
	name = pop();
	tp = pop();

	tp->size = size->u.i;
	tp->align = align->u.i;
	/*
	 * type is the first field of Symbol so we can obtain the
	 * address of the symbol from the address of the type.
	 * We have to do this because composed returns the pointer
	 * to the type, but in this function we also need the
	 * symbol to store the name.
	 */
	sym = (Symbol *) tp;
	sym->name = name;

	delnode(align);
	delnode(size);
}

static void
array(void)
{
	Type *tp, *base;
	Node *size;

	size = pop();
	base = pop();
	tp = pop();
	tp->size = size->u.i;
	tp->align = base->align;

	delnode(size);
}

static void
decl(Symbol *sym)
{
	switch (sym->kind) {
	case EXTRN:
		label(sym);
		break;
	case GLOB:
	case PRIVAT:
	case LOCAL:
		label(sym);
		if (!ininit)
			allocdata(&sym->type);
		break;
	case AUTO:
	case REG:
		if (funpars >= 0) {
			if (funpars == NR_FUNPARAM)
				error(EOUTPAR);
			params[funpars++] = sym;
			break;
		}
		break;
	}
}

static void
vardecl(void)
{
	Type *tp;
	Node *np;
	Symbol *sym;
	char *name;

	name = pop();
	tp = pop();
	np = pop();

	sym = np->u.sym;
	/*
	 * We have to free sym->name because in tentative declarations
	 * we can multiple declarations of the same symbol, and in this
	 * case our parser will allocate twice the memory
	 */
	free(sym->name);
	sym->name = name;
	sym->type = *tp;
	sym->kind = sclass;

	if (ininit)
		sym->type.flags |= INITF;

	if ((tp->flags & FUNF) == 0) {
		decl(sym);
	} else {
		curfun = sym;
		label(sym);
	}

	delnode(np);
}

static void
flddecl(void)
{
	Node *off, *np;
	char *name;
	Type *tp;
	Symbol *sym;

	off = pop();
	name = pop();
	tp = pop();
	np = pop();

	sym = np->u.sym;
	sym->u.off = off->u.i;
	sym->name = name;
	sym->type = *tp;

	delnode(np);
	delnode(off);
}

static void
labeldcl( void)
{
	Node *np;
	Symbol *sym;

	np = pop();
	sym = np->u.sym;
	delnode(np);
	nextline();
	stmtp->label = sym;
	sym->u.label = stmtp;
	
}

static void
addnode(Node *np)
{
	if (!curfun->u.label)
		curfun->u.label = np;
	else
		stmtp->stmt = np;
	stmtp = np;
}

static void
stmt(void)
{
	Node *np;

	np = pop();
	if (ininit) {
		data(np);
		deltree(np);
		return;
	}
	addnode(np);
}

static void
beginfun(void)
{
	memset(params, 0, sizeof(params));
	funpars = 0;
	pushctx();
}

static void
endfun(void)
{
	Node *np;

	np = newnode();
	np->op = ONOP;
	addnode(np);
	curfun = NULL;
	funpars = -1;
	endf = 1;
	popctx();
}

void
parse(void)
{
	endf = 0;
	while (!endf && nextline())
		/* nothing */;
	if (ferror(stdin))
		error(EFERROR, strerror(errno));
}
