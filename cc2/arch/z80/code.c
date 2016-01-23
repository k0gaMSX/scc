
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

static void
emitsym(Symbol *sym)
{
	/*In z80 we can ignore the aligment */
	if (sym->type.flags & STRF) {
		fputs(sym->u.s, stdout);
		free(sym->u.s);
		sym->u.s = NULL;
	} else {
		switch (sym->type.size) {
		case 1:
			printf("%02X", (int) (sym->u.i & 0xFF));
			break;
		case 2:
			printf("%04X", (int) (sym->u.i & 0xFFFF));
			break;
		case 4:
			printf("%08X", (long) (sym->u.i & 0xFFFFFFFF));
			break;
		default:
			abort();
		}
	}
}

static void
emittree(Node *np)
{
	if (!np)
		return;
	if (np->op == OSYM) {
		emitsym(np->sym);
	} else {
		emit(np->left);
		printf(" %c ", np->op);
		emit(np->right);
	}
}

void
emit(Node *np)
{
	char *s;

	/*
	 * In z80 we can ignore the alignment
	 */
	segment(DATASEG);
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
}

void
writeout(void)
{
}
