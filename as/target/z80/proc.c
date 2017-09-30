static char sccsid[] = "@(#) ./as/target/i386/proc.c";

#include <stdlib.h>

#include "../../../inc/scc.h"
#include "../../as.h"
#include "../x80/proc.h"

void
iarch(void)
{
	static struct {
		char *name;
		char type;
	} regs[] = {
		"AF", AREG_AF,
		"A", AREG_A,
		"F", AREG_F,

		"BC", AREG_BC,
		"B", AREG_B,
		"C", AREG_C,

		"HL", AREG_HL,
		"H", AREG_H,
		"L", AREG_L,

		"DE", AREG_DE,
		"D", AREG_D,
		"E", AREG_E,

		"IX", AREG_IX,
		"IXL", AREG_IXL,
		"IXH", AREG_IXH,

		"IY", AREG_IY,
		"IYL", AREG_IYL,
		"IYH", AREG_IYH,

		"R", AREG_R,
		"I", AREG_I,
		"AF_", AREG_AF_,

		NULL,
	}, *bp;

	for (bp = regs; bp->name; ++bp) {
		Symbol *sym = lookup(bp->name, TREG);
		sym->argtype = bp->type;
	}
}

int
match(Op *op, Node **args)
{
	unsigned char *p;
	int arg;
	Node *np;

	if (!op->args)
		return args == NULL;

	for (p = op->args; (arg = *p) && *args; ++p) {
		if (arg & AREP)
			--p;
		np = *args++;
		switch (arg & ~AREP) {
		case AREG_RCLASS:
			if (np->op != AREG)
				return 0;
			if (!rclass(np->sym->argtype))
				return 0;
			break;
		case AREG_PCLASS:
			if (np->op != AREG)
				return 0;
			if (!pclass(np->sym->argtype))
				return 0;
			break;
		case AREG_QCLASS:
			if (np->op != AREG)
				return 0;
			if (!qclass(np->sym->argtype))
				return 0;
			break;
		case AIMM8:
		case AIMM16:
		case AIMM32:
		case AIMM64:
			if (np->addr != AIMM)
				return 0;
			if (toobig(np, arg))
				error("overflow in immediate operand");
			break;
		default:
			abort();
		}
	}

	return (!arg || arg & AREP) && !*args;
}
