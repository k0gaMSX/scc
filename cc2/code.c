
#include <stdlib.h>

#include "arch.h"
#include "cc2.h"

Inst *pc, *prog;

static void
nextpc(void)
{
        Inst *new;

        new = malloc(sizeof(*new)); /* TODO: create an arena */

        if (!pc) {
                new->next = NULL;
                prog = new;
        } else {
                new->next = pc->next;
                pc->next = new;
        }

        new->prev = pc;
        new->to.kind = new->from2.kind = new->from1.kind = SNONE;
        pc = new;
}

static void
addr(Node *np, Addr *addr)
{
	switch (addr->kind = np->op) {
	case SREG:
		addr->u.reg = np->u.reg;
		break;
	case SCONST:
		addr->u.i = np->u.i;
		break;
	case SLABEL:
	case SAUTO:
	case STMP:
		addr->u.sym = np->u.sym;
		break;
	default:
		abort();
	}
}

void
setlabel(Symbol *sym)
{
	if (!sym)
		return;
	code(0, NULL, NULL, NULL);
	pc->label = sym;
}

void
code(int op, Node *to, Node *from1, Node *from2)
{
	nextpc();
	if (from1)
		addr(from1, &pc->from1);
	if (from2)
		addr(from2, &pc->from2);
	if (to)
		addr(to, &pc->to);
	pc->op = op;
}

void
delcode(void)
{
        Inst *prev = pc->prev, *next = pc->next;

        free(pc);
        if (!prev) {
                pc = next;
                prog = NULL;
        } else {
                pc = prev;
                prev->next = next;
                if (next)
                        next->prev = prev;
        }
}
