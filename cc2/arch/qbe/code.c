
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
			s = "z\t%llu\t";
			break;
		}
	}
	printf(s, (unsigned long long) tp->size);
}

void
defsym(Symbol *sym, int alloc)
{
	if (!alloc)
		return;
	if (sym->kind == GLOB)
		fputs("export ", stdout);
	printf("data %c%s = {\n", sigil(sym), sym->name);
	if (sym->type.flags & INITF)
		return;
	putchar('\t');
	size2asm(&sym->type);
	puts("0\n}");
}

void
data(Node *np)
{
}

void
writeout(void)
{
}
