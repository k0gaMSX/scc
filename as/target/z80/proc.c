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
		Symbol *sym = lookup(bp->name);
		sym->flags = FREG;
		sym->argtype = bp->type;
	}
}

int
match(Op *op, Node **args)
{
	unsigned char *p;
	int arg;
	Node *np;
	int (*class)(int);

	if (!op->args)
		return args == NULL;

	for (p = op->args; arg = *p; ++p) {
		if (arg & AREP)
			--p;
		if ((np = *args++) == NULL)
			return (arg & (AREP|AOPT)) != 0;

		arg &= ~(AREP|AOPT);
		switch (arg) {
		case AINDER_HL:
			if (np->addr != AINDIR)
				return 0;
			if (np->left->sym->argtype != AREG_HL)
				return 0;
			break;
		case AREG_A:
		case AREG_HL:
		case AREG_IY:
		case AREG_IX:
		case AREG_SP:
			if (np->addr != AREG || np->sym->argtype != arg)
				return 0;
			break;
		case AREG_RCLASS:
			class = rclass;
			goto register_class;
		case AREG_PCLASS:
			class = pclass;
			goto register_class;
		case AREG_QCLASS:
			class = qclass;
			goto register_class;
		case AREG_DDCLASS:
			class = ddclass;
		register_class:
			if (np->addr != AREG)
				return 0;
			if (!(*class)(np->sym->argtype))
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
		case ASYM:
			if (np->addr != AIMM || np->op != IDEN)
				return 0;
			break;
		case ASTR:
			if (np->addr != ASTR)
				return 0;
			break;
		default:
			abort();
		}
	}

	return *args == NULL;
}
