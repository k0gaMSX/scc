
#include "arch.h"
#include "cc2.h"

Node *
optm(Node *np)
{
	Node *dst;

	switch (np->op) {
	case OJMP:
	case OBRANCH:
		dst = np->u.sym->u.stmt;
		if (dst->op == OJMP)
			np->u.sym = dst->u.sym;
		break;
	}
	return np;
}
