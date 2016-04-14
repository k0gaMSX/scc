
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
			sprintf(name, "%c%s", c, sym->name);
			return sym->name;
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

	/* In qbe we can ignore the aligment because it handles it */

	if (tp->flags & STRF) {
		s = "b\t";
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
			s = "l\t";
			break;
		default:
			abort();
		}
	}
	fputs(s, stdout);
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
}

void
defvar(Symbol *sym)
{
}

void
data(Node *np)
{
	putchar('\t');
	size2asm(&np->type);
	emittree(np);
	putchar(',');
	putchar('\n');
}

void
writeout(void)
{
	if (curfun->kind == GLOB)
		fputs("export ", stdout);
	printf("function %s(", symname(curfun));
	puts("){");
	puts("}");
}

void
endinit(void)
{
	puts("}");
}
