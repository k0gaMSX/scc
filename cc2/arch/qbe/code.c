
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arch.h"
#include "../../cc2.h"
#include "../../../inc/sizes.h"

#define ADDR_LEN (IDENTSIZ+64)

static void binary(void), load(void), store(void);

static struct opdata {
	void (*fun)(void);
	char *txt;
	char letter;
} optbl [] = {
	[ASLOAD] =  {.fun = load,   .txt = "load", .letter = 'w'},
	[ASASSIG] = {.fun = store,  .txt = "store", .letter = 'w'},

	[ASADDW]  =  {.fun = binary, .txt = "add", .letter = 'w'},
	[ASSUBW]  =  {.fun = binary, .txt = "sub", .letter = 'w'},
	[ASMULW]  =  {.fun = binary, .txt = "mul", .letter = 'w'},
	[ASMODW]  =  {.fun = binary, .txt = "rem", .letter = 'w'},
	[ASUMODW] =  {.fun = binary, .txt = "urem", .letter = 'w'},
	[ASDIVW]  =  {.fun = binary, .txt = "div", .letter = 'w'},
	[ASUDIVW] =  {.fun = binary, .txt = "udiv", .letter = 'w'},
	[ASSHLW]  =  {.fun = binary, .txt = "shl", .letter = 'w'},
	[ASSHRW]  =  {.fun = binary, .txt = "shrs", .letter = 'w'},
	[ASUSHRW] =  {.fun = binary, .txt = "shrz", .letter = 'w'},
	[ASLTW]   =  {.fun = binary, .txt = "csltw", .letter = 'w'},
	[ASULTW]  =  {.fun = binary, .txt = "cultw", .letter = 'w'},
	[ASGTW]   =  {.fun = binary, .txt = "csgtw", .letter = 'w'},
	[ASUGTW]  =  {.fun = binary, .txt = "cugtw", .letter = 'w'},
	[ASLEW]   =  {.fun = binary, .txt = "cslew", .letter = 'w'},
	[ASULEW]  =  {.fun = binary, .txt = "culew", .letter = 'w'},
	[ASGEW]   =  {.fun = binary, .txt = "csgew", .letter = 'w'},
	[ASUGEW]  =  {.fun = binary, .txt = "cugew", .letter = 'w'},
	[ASEQW]   =  {.fun = binary, .txt = "ceqw", .letter = 'w'},
	[ASNEW]   =  {.fun = binary, .txt = "cnew", .letter = 'w'},
	[ASBANDW] =  {.fun = binary, .txt = "and", .letter = 'w'},
	[ASBORW]  =  {.fun = binary, .txt = "or", .letter = 'w'},
	[ASBXORW] =  {.fun = binary, .txt = "xor", .letter = 'w'},

	[ASADDL]  =  {.fun = binary, .txt = "add", .letter = 'l'},
	[ASSUBL]  =  {.fun = binary, .txt = "sub", .letter = 'l'},
	[ASMULL]  =  {.fun = binary, .txt = "mul", .letter = 'l'},
	[ASMODL]  =  {.fun = binary, .txt = "rem", .letter = 'l'},
	[ASUMODL] =  {.fun = binary, .txt = "urem", .letter = 'l'},
	[ASDIVL]  =  {.fun = binary, .txt = "div", .letter = 'l'},
	[ASUDIVL] =  {.fun = binary, .txt = "udiv", .letter = 'l'},
	[ASSHLL]  =  {.fun = binary, .txt = "shl", .letter = 'l'},
	[ASSHRL]  =  {.fun = binary, .txt = "shrs", .letter = 'l'},
	[ASUSHRL] =  {.fun = binary, .txt = "shrz", .letter = 'l'},
	[ASLTL]   =  {.fun = binary, .txt = "csltl", .letter = 'w'},
	[ASULTL]  =  {.fun = binary, .txt = "cultl", .letter = 'w'},
	[ASGTL]   =  {.fun = binary, .txt = "csgtl", .letter = 'w'},
	[ASUGTL]  =  {.fun = binary, .txt = "cugtl", .letter = 'w'},
	[ASLEL]   =  {.fun = binary, .txt = "cslel", .letter = 'w'},
	[ASULEL]  =  {.fun = binary, .txt = "culel", .letter = 'w'},
	[ASGEL]   =  {.fun = binary, .txt = "csgel", .letter = 'w'},
	[ASUGEL]  =  {.fun = binary, .txt = "cugel", .letter = 'w'},
	[ASEQL]   =  {.fun = binary, .txt = "ceql", .letter = 'w'},
	[ASNEL]   =  {.fun = binary, .txt = "cnel", .letter = 'w'},
	[ASBANDL] =  {.fun = binary, .txt = "and", .letter = 'l'},
	[ASBORL]  =  {.fun = binary, .txt = "or", .letter = 'l'},
	[ASBXORL] =  {.fun = binary, .txt = "xor", .letter = 'l'},

	[ASADDS]  =  {.fun = binary, .txt = "add", .letter = 's'},
	[ASSUBS]  =  {.fun = binary, .txt = "sub", .letter = 's'},
	[ASMULS]  =  {.fun = binary, .txt = "mul", .letter = 's'},
	[ASDIVS]  =  {.fun = binary, .txt = "div", .letter = 's'},
	[ASLTS]   =  {.fun = binary, .txt = "clts", .letter = 'w'},
	[ASGTS]   =  {.fun = binary, .txt = "cgts", .letter = 'w'},
	[ASLES]   =  {.fun = binary, .txt = "cles", .letter = 'w'},
	[ASGES]   =  {.fun = binary, .txt = "cges", .letter = 'w'},
	[ASEQS]   =  {.fun = binary, .txt = "ceqs", .letter = 'w'},
	[ASNES]   =  {.fun = binary, .txt = "cnes", .letter = 'w'},

	[ASADDD]  =  {.fun = binary, .txt = "add", .letter = 'd'},
	[ASSUBD]  =  {.fun = binary, .txt = "sub", .letter = 'd'},
	[ASMULD]  =  {.fun = binary, .txt = "mul", .letter = 'd'},
	[ASDIVD]  =  {.fun = binary, .txt = "div", .letter = 'd'},
	[ASLTD]   =  {.fun = binary, .txt = "cltd", .letter = 'w'},
	[ASGTD]   =  {.fun = binary, .txt = "cgtd", .letter = 'w'},
	[ASLED]   =  {.fun = binary, .txt = "cled", .letter = 'w'},
	[ASGED]   =  {.fun = binary, .txt = "cged", .letter = 'w'},
	[ASEQD]   =  {.fun = binary, .txt = "ceqd", .letter = 'w'},
	[ASNED]   =  {.fun = binary, .txt = "cned", .letter = 'w'},
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
	case SEXTRN:
	case SGLOB:
	case SPRIV:
	case SLOCAL:
		return '$';
	case SAUTO:
	case STMP:
		return '%';
	case SLABEL:
		return '@';
	default:
		abort();
	}
}

static char *
symname(Symbol *sym)
{
	static char name[ADDR_LEN];
	static unsigned id;
	char c = sigil(sym);

	if (sym->name) {
		switch (sym->kind) {
		case SEXTRN:
		case SGLOB:
			sprintf(name, "%c%s", c, sym->name);
			return name;
		case SPRIV:
		case SAUTO:
			sprintf(name, "%c%s.%u", c, sym->name, sym->id);
			return name;
		default:
			abort();
		}
	}

	if (sym->numid == 0 && (sym->numid = ++id) == 0)
		error(EIDOVER);
	sprintf(name, "%c.%u", c, sym->numid);

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
		case 0:
			return "";
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
	if (sym->kind == SEXTRN)
		return;
	if (sym->kind == SGLOB)
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
	char *sep;

	if (curfun->kind == SGLOB)
		fputs("export ", stdout);
	printf("function %s %s(", size2asm(&curfun->rtype), symname(curfun));

	for (sep = "", p = locals; p; p = p->next, sep = ",") {
		if ((p->type.flags & PARF) == 0)
			break;
		printf("%s%s %s", sep, size2asm(&p->type), symname(p));
	}
	puts(")");

	for ( ; p && p->id != TMPSYM; p = p->next) {
		tp = &p->type;
		printf("\t%s %s=\talloc%lld\t%lld\n",
		       symname(p), size2asm(tp),
		       (long long) tp->size, (long long) tp->align);
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
	switch (a->kind) {
	case SAUTO:
	case SLABEL:
	case STMP:
		return symname(a->u.sym);
	default:
		abort();
	}
}

static void
binary(void)
{
	struct opdata *p = &optbl[pc->op];
	char to[ADDR_LEN], from1[ADDR_LEN], from2[ADDR_LEN];

	strcpy(to, addr2txt(&pc->to));
	strcpy(from1, addr2txt(&pc->from1));
	strcpy(from2, addr2txt(&pc->from2));
	printf("\t%s %c=\t%s\t%s,%s\n", to, p->letter, p->txt, from1, from2);
}

static void
store(void)
{
	printf("\t\t%s%c\t", optbl[pc->op].txt, 'w'),
	fputs(addr2txt(&pc->from1), stdout);
	putchar(',');
	fputs(addr2txt(&pc->to), stdout);
	putchar('\n');
}

static void
load(void)
{
	printf("\t%s %c=\t", addr2txt(&pc->to), 'w');
	printf("%s\t%s\n", optbl[pc->op].txt, addr2txt(&pc->from1));
}

void
endinit(void)
{
	puts("}");
}
