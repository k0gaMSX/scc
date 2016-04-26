
#include <stddef.h>

#include "arch.h"
#include "cc2.h"

Node *
optm(Node *np)
{
	Node *p, *dst, *next = np->next;
	Symbol *sym, *osym;

	switch (np->op) {
	case ONOP:
		if (next && next->op == ONOP) {
			sym = np->u.sym;
			osym = next->u.sym;
			osym->id = sym->id;
			osym->numid = sym->id;
			osym->u.stmt = sym->u.stmt;
			return NULL;
		}
		break;
	case OJMP:
	case OBRANCH:
		for (;;) {
			dst = np->u.sym->u.stmt;
			if (dst->op != OJMP)
				break;
			np->u.sym = dst->u.sym;
		}
		for (p = np->next; p; p = p->next) {
			if (p == dst)
				return NULL;
			if (p->op == ONOP ||
			    p->op == OBLOOP ||
			    p->op == OELOOP) {
				continue;
			}
			break;
		}
		break;
	}
	return np;
}
