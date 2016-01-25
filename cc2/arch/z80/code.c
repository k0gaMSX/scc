
#include <stdio.h>
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"
#include "../../../inc/sizes.h"

enum segment {
	CODESEG,
	DATASEG,
	BSSSEG,
	NOSEG
};

static int curseg = NOSEG;

static void
segment(int seg)
{
	static char *txt[] = {
		[CODESEG] = "\tCSEG\n",
		[DATASEG] = "\tDSEG\n",
		[BSSSEG] = "\tXSEG\n",
	};

	if (seg == curseg)
		return;
	fputs(txt[seg], stdout);
	curseg = seg;
}

static char *
symname(Symbol *sym)
{
	static char name[IDENTSIZ+1];
	static unsigned short id;
	int k = sym->kind;

	if (sym->name) {
		switch (sym->kind) {
		case GLOB:
		case EXTRN:
			snprintf(name, sizeof(name), "_%s", sym->name);
			return name;;
		case PRIVAT:
			return sym->name;
		}
	}

	if (sym->numid == 0) {
		if ((sym->numid = ++id) == 0)
			error(EIDOVER);
	}
	sprintf(name, ".%d", sym->numid);

	return name;
}

void
code(int op, Node *to, Node *from)
{
}

void
label(Symbol *sym)
{
	int seg, flags = sym->type.flags;
	char *name = symname(sym);

	putchar('\n');
	if (flags & FUNF)
		seg = CODESEG;
	else if (flags & INITF)
		seg = DATASEG;
	else
		seg = BSSSEG;
	segment(seg);

	switch (sym->kind) {
	case EXTRN:
		printf("\tEXTRN %s\n", name);
		return;
	case GLOB:
		printf("\tPUBLIC %s\n", name);
		break;
	}

	printf("%s:\n", name);
}

static void
emitstring(Node *np)
{
	/*In z80 we can ignore the aligment */
	printf("\"%s\"", np->u.s);
	free(np->u.s);
	np->u.s = NULL;
}

static void
emitconst(Node *np)
{
	switch (np->type.size) {
	case 1:
		printf("%02X", (int) np->u.i & 0xFF);
		break;
	case 2:
		printf("%04X", (int) np->u.i & 0xFFFF);
		break;
	case 4:
		printf("%08X", (long) np->u.i & 0xFFFFFFFF);
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
		emitstring(np);
		break;
	case OCONST:
		emitconst(np);
		break;
	case OADDR:
		emittree(np->left);
		break;
	case MEM:
		fputs(symname(np->u.sym), stdout);
		break;
	default:
		emittree(np->left);
		printf(" %c ", np->op);
		emittree(np->right);
		break;
	}
}


static void
size2asm(Type *tp)
{
	char *s;

	/*
	 * In z80 we can ignore the alignment
	 */
	switch (tp->size) {
	case 1:
		s = "\tDB\t";
		break;
	case 2:
		s = "\tDW\t";
		break;
	case 4:
		s = "\tDD\t";
		break;
	default:
		s = (tp->flags & STRF) ? "\tTEXT\t" : "\tDS\t%llu,";
		break;
	}
	printf(s, (unsigned long long) tp->size);
}

void
allocdata(Type *tp)
{
	size2asm(tp);
	puts("0");
}

void
data(Node *np)
{
	size2asm(&np->type);
	emittree(np);
	putchar('\n');
}

void
writeout(void)
{
}
