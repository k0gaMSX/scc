
#include <stdio.h>
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"

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

void
code(int op, Node *to, Node *from)
{
}

void
label(Symbol *sym)
{
	int seg, flags = sym->type.flags;

	if (flags & FUNF)
		seg = CODESEG;
	else if (flags & INITF)
		seg = DATASEG;
	else
		seg = BSSSEG;
	segment(seg);

	printf("%s:\n", symname(sym));
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

void
data(Node *np)
{
	char *s;

	/*
	 * In z80 we can ignore the alignment
	 */
	switch (np->type.size) {
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
		s = "\tDS\t";
		break;
	}
	fputs(s, stdout);
	emittree(np);
	putchar('\n');
}

void
writeout(void)
{
}
