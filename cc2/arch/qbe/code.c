
#include <stdio.h>
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"
#include "../../../inc/sizes.h"

static void inst3(void);

static struct opdata {
	void (*fun)(void);
	char *txt;
} optbl [] = {
	[OADD]  = {.fun = inst3, .txt = "add"},
	[OSUB]  = {.fun = inst3, .txt = "sub"},
	[OMUL]  = {.fun = inst3, .txt = "mul"},
	[OMOD]  = {.fun = inst3, .txt = "rem"},
	[ODIV]  = {.fun = inst3, .txt = "div"},
	[OSHL]  = {.fun = inst3, .txt = "shl"},
	[OSHR]  = {.fun = inst3, .txt = "shr"},
	[OLT]   = {.fun = inst3, .txt = "clt"},
	[OGT]   = {.fun = inst3, .txt = "cgt"},
	[OLE]   = {.fun = inst3, .txt = "cle"},
	[OGE]   = {.fun = inst3, .txt = "cge"},
	[OEQ]   = {.fun = inst3, .txt = "ceq"},
	[ONE]   = {.fun = inst3, .txt = "cne"},
	[OBAND] = {.fun = inst3, .txt = "and"},
	[OBOR]  = {.fun = inst3, .txt = "or"},
	[OBXOR] = {.fun = inst3, .txt = "xor"}
};

/*
 * : is for user-defined Aggregate Types
 * $ is for globals (represented by a pointer)
 * % is for function-scope temporaries
 * @ is for block labels
 */
static char
sigil(Symbol *sym)
{
	switch (sym->kind) {
	case EXTRN:
	case GLOB:
	case PRIVAT:
	case LOCAL:
		return '$';
	case AUTO:
	case REG:
		return '%';
	default:
		abort();
	}
}

static char *
symname(Symbol *sym)
{
	static char name[IDENTSIZ+2];
	static unsigned short id;
	char c = sigil(sym);

	if (sym->name) {
		switch (sym->kind) {
		case EXTRN:
		case GLOB:
		case PRIVAT:
		case AUTO:
			sprintf(name, "%c%s", c, sym->name);
			return name;
		default:
			abort();
		}
	}

	if (sym->numid == 0 && (sym->numid = ++id) == 0)
		error(EIDOVER);
	sprintf(name, "%c.%d", c, sym->numid);

	return name;
}

static void
emitconst(Node *np)
{
	switch (np->type.size) {
	case 1:
		printf("%d", (int) np->u.i & 0xFF);
		break;
	case 2:
		printf("%d", (int) np->u.i & 0xFFFF);
		break;
	case 4:
		printf("%ld", (long) np->u.i & 0xFFFFFFFF);
		break;
        case 8:
                printf("%lld", (long long) np->u.i & 0xFFFFFFFF);
                break;
	default:
		abort();
	}
}

static void
emittree(Node *np)
{
	if (!np)
		return;

	switch (np->op) {
	case OSTRING:
		printf("\"%s\"", np->u.s);
		free(np->u.s);
		np->u.s = NULL;
		break;
	case OCONST:
		emitconst(np);
		break;
	case OADDR:
		emittree(np->left);
		break;
	case OMEM:
		fputs(symname(np->u.sym), stdout);
		break;
	default:
		emittree(np->left);
		printf(" %c ", np->op);
		emittree(np->right);
		break;
	}
}

static char *
size2asm(Type *tp)
{
	if (tp->flags & STRF) {
		return "b";
	} else {
		switch (tp->size) {
		case 1:
			return "b";
		case 2:
			return "h";
		case 4:
			return "w";
		case 8:
			return "l";
		default:
			abort();
		}
	}
}

void
defglobal(Symbol *sym)
{
	if (sym->kind == EXTRN)
		return;
	if (sym->kind == GLOB)
		fputs("export ", stdout);
	printf("data %s = {\n", symname(sym));
	if (sym->type.flags & INITF)
		return;
	printf("\tz\t%llu\n}\n", (unsigned long long) sym->type.size);
}

void
defpar(Symbol *sym)
{
	sym->type.flags |= PARF;
}

void
defvar(Symbol *sym)
{
}

void
data(Node *np)
{
	printf("\t%s\t", size2asm(&np->type));
	emittree(np);
	putchar(',');
	putchar('\n');
}

void
writeout(void)
{
	Symbol *p;
	Type *tp;

	if (curfun->kind == GLOB)
		fputs("export ", stdout);
	printf("function %s %s(\n", size2asm(&curfun->rtype), symname(curfun));

	for (p = locals; p && p->type.flags & PARF; p = p->next)
		printf("\t%s %s,\n", size2asm(&p->type), symname(p));

	puts(")\n{");

	for ( ; p; p = p->next) {
		tp = &p->type;
		printf("\t%s %s= alloc%d %d\n",
		       symname(p), size2asm(tp), tp->size, tp->align);
	}

	for (pc = prog; pc; pc = pc->next) {
		if (pc->label)
			printf("%s:\n", symname(pc->label));
		(*optbl[pc->op].fun)();
	}

	puts("}");
}

static char *
addr2txt(Addr *a)
{
	static char buff[40];

	switch (a->kind) {
	case AUTO:
	case LABEL:
		return symname(a->u.sym);
	default:
		abort();
	}
}

static void
inst3(void)
{
	printf("\t%s %c= %s\t%s,%s\n",
	       addr2txt(&pc->to),
	       optbl[pc->op].txt,
	       addr2txt(&pc->from1), addr2txt(&pc->from2));
}

void
endinit(void)
{
	puts("}");
}
