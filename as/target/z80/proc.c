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
		"AF'", AREG_AF_,
		"SP", AREG_SP,

		"NZ", AREG_NZ,
		"Z", AREG_Z,
		"NC", AREG_NC,
		"PO", AREG_PO,
		"PE", AREG_PE,
		"P", AREG_P,
		"M", AREG_M,

		NULL,
	}, *bp;

	for (bp = regs; bp->name; ++bp) {
		Symbol *sym = lookup(bp->name);
		sym->flags = FREG;
		sym->value = bp->type;
	}
}

int
match(Op *op, Node **args)
{
	unsigned char *p;
	int arg, class;
	Node *np;

	if (!op->args)
		return args == NULL;

	for (p = op->args; arg = *p; ++p) {
		if (arg & AREP)
			--p;
		if ((np = *args++) == NULL)
			return (arg & (AREP|AOPT)) != 0;

		arg &= ~(AREP|AOPT);
		switch (arg) {
		case AINDER_C:
			arg = AREG_C;
			goto indirect;
		case AINDER_HL:
			arg = AREG_HL;
			goto indirect;
		case AINDER_DE:
			arg = AREG_DE;
			goto indirect;
		case AINDER_BC:
			arg = AREG_BC;
			goto indirect;
		case AINDER_IX:
			arg = AREG_IX;
			goto indirect;
		case AINDER_IY:
			arg = AREG_IY;
			goto indirect;
		case AINDER_SP:
			arg = AREG_SP;
		indirect:
			if (np->addr != AINDIR)
				return 0;
			np = np->left;
		case AREG_A:
		case AREG_I:
		case AREG_R:
		case AREG_F:
		case AREG_HL:
		case AREG_BC:
		case AREG_DE:
		case AREG_IY:
		case AREG_IX:
		case AREG_SP:
		case AREG_AF:
		case AREG_AF_:
			if (np->addr != AREG || np->sym->value != arg)
				return 0;
			break;
		case AREG_RCLASS:
			class = RCLASS;
			goto check_class;
		case AREG_PCLASS:
			class = PCLASS;
			goto check_class;
		case AREG_QCLASS:
			class = QCLASS;
			goto check_class;
		case AREG_QQCLASS:
			class = QQCLASS;
			goto check_class;
		case AREG_PPCLASS:
			class = PPCLASS;
			goto check_class;
		case AREG_RRCLASS:
			class = RRCLASS;
			goto check_class;
		case AREG_CCCLASS:
			class = CCCLASS;
			goto check_class;
		case AREG_DDCLASS:
			class = DDCLASS;
		check_class:
			if ((getclass(np) & class) == 0)
				return 0;
			break;
		case AINDEX_IY:
			arg = AREG_IY;
			goto index_address;
		case AINDEX_IX:
			arg = AREG_IX;
		index_address:
			if (np->addr != AINDEX)
				return 0;
			if (np->left->left->sym->value != arg)
				return 0;
			if (toobig(np, arg))
				error("overflow in index");
			break;
		case ARST:
			if (np->addr != AIMM)
				return 0;
			if ((np->sym->value & ~0x38) != 0)
				return 0;
			break;
		case AZERO:
		case AIMM3:
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
		case ADIRECT:
		case ASTR:
			if (np->addr != arg)
				return 0;
			break;
		default:
			abort();
		}
	}

	return *args == NULL;
}
