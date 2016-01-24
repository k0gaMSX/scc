
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
                parameter, constant, aggregate;

typedef void evalfun(void);
static evalfun vardecl, beginfun, endfun, endpars, stmt, begininit,
               endinit, array;

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
	[MEMBER]      = {NULL, symbol},
	[LABEL]       = {NULL, symbol},

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
	[VECTOR]      = {array, aggregate},
	[UNION]       = {NULL, NULL},
	[STRUCT]      = {NULL, NULL},

	[ONAME]       = {NULL, getname},
	['{']         = {beginfun},
	['}']         = {endfun},
	['(']         = {begininit},
	[')']         = {endinit},
	[OEPARS]      = {endpars},
	[OSTMT]       = {stmt},

	[OCPL]        = {NULL, unary},
	[ONEG]        = {NULL, unary},
	[OFIELD]      = {NULL, unary},
	[OADDR]       = {NULL, unary},
	[OAND]        = {NULL, unary},
	[OOR]         = {NULL, unary},
	[OPTR]        = {NULL, unary},
	[OCAST]       = {NULL, unary},
	[OPAR ]       = {NULL, unary},

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
	[OASSIG]      = {NULL, binary},
	[OCOMMA]      = {NULL, binary},

	[OASK]        = {NULL, ternary},
	[OCALL]       = {NULL, call},

	[OCONST]      = NULL, constant,

	[OCASE]       = NULL,
	[ODEFAULT]    = NULL,
	[OTABLE]      = NULL,
	[OSWITCH]     = NULL
};

static void *stack[STACKSIZ], **sp = stack;
static Symbol *lastsym, *curfun;
static Symbol *params[NR_FUNPARAM];
static int funpars = -1, sclass, callpars, ininit;
static Node *stmtp, *callp;

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

static unsigned short
newid(void)
{
	static unsigned short id;

	if (++id == 0)
		error(EIDOVER);
	return id;
}

static void
type(char *token, union tokenop u)
{
	push(u.arg);
}

static void
aggregate(char *token, union tokenop u)
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
	ask->right = pop();
	push(ask);
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
	fun->op = OCALL;
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
begininit(void)
{
	ininit = 1;
	lastsym->type.flags |= INITF;
	label(lastsym);
}

static void
endinit(void)
{
	ininit = 0;
}

static void
beginfun(void)
{
	if (curfun)
		error(ESYNTAX);
	memset(params, 0, sizeof(params));
	funpars = 0;
	curfun = lastsym;
	pushctx();
}

static void
endfun(void)
{
	if (!curfun)
		error(ESYNTAX);
	popctx();
	curfun = NULL;
	funpars = -1;
}

static void
endpars(void)
{
	if (!curfun || funpars == -1)
		error(ESYNTAX);
	funpars = -1;
}

static void
array(void)
{
	Type *tp, *base;
	Node *np;

	np = pop();
	base = pop();
	tp = pop();
}

static void
vardecl(void)
{
	Type *tp;
	Node *np;
	Symbol *sym;
	char *name;

	if (lastsym && (lastsym->type.flags & INITF) == 0) {
		switch (lastsym->kind) {
		case EXTRN:
			label(lastsym);
			break;
		case GLOB:
		case PRIVAT:
		case LOCAL:
			label(lastsym);
			allocdata(&lastsym->type);
			break;
		}
	}

	name = pop();
	tp = pop();
	np = pop();

	sym = np->u.sym;
	sym->name = name;
	sym->type = *tp;
	sym->kind = sclass;
	lastsym = sym;
	if (!name)
		sym->numid = newid();

	if (funpars >= 0) {
		if (funpars == NR_FUNPARAM)
			error(EOUTPAR);
		params[funpars++] = sym;
	}
	delnode(np);
}

static void
stmt(void)
{
	static Node *lastp;
	Node *np = pop();

	if (ininit) {
		data(np);
		deltree(np);
		return;
	}
	if (!stmtp)
		stmtp = np;
	else
		lastp->stmt = np;
	lastp = np;
}

void
parse(void)
{
	char line[MAXLINE], *t;
	size_t len;
	int c;
	struct decoc *dp;

	for (;;) {
		if (!fgets(line, sizeof(line), stdin))
			break;
		if ((len = strlen(line)) == 0 || line[0] == '\n')
			continue;
		if (line[len-1] != '\n')
			error(ELNLINE);
		line[len-1] = '\0';

		c = *line;
		for (t = strtok(line, "\t\n"); t; t = strtok(NULL, "\t\n")) {
			dp = &optbl[*t];
			if (!dp->parse)
				break;
			(*dp->parse)(t, dp->u);
		}

		(*optbl[c].eval)();
		if (sp != stack)
			error(ESTACKA);
		if (c == '}')
			break;
	}

	if (ferror(stdin))
		error(EFERROR, strerror(errno));
}
