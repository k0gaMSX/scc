
#include <stdio.h>
#include <stdlib.h>

#include "arch.h"
#include "../../cc2.h"
#include "../../../inc/sizes.h"

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

static void
size2asm(Type *tp)
{
	char *s;

	/* In qbe we can ignore the aligment because it handles it */

	if (tp->flags & STRF) {
		abort();
	} else {
		switch (tp->size) {
		case 1:
			s = "b\t";
			break;
		case 2:
			s = "h\t";
			break;
		case 4:
			s = "w\t";
			break;
		case 8:
			s = "q\t";
			break;
		default:
			abort();
		}
	}
	fputs(s, stdout);
}

void
defsym(Symbol *sym, int alloc)
{
	if (!alloc)
		return;
	if (sym->kind == GLOB)
		fputs("export ", stdout);
	printf("data $%s = {\n", sym->name);
	if (sym->type.flags & INITF)
		return;
	printf("\tz\t%llu\n}\n", (unsigned long long) sym->type.size);
}

void
data(Node *np)
{
}

void
writeout(void)
{
}
