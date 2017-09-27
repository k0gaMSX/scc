static char sccsid[] = "@(#) ./as/target/i386/proc.c";

#include <stdlib.h>

#include "../../../inc/scc.h"
#include "../../as.h"
#include "../x86/proc.h"

void
iarch(void)
{
	static struct {
		char *name;
		char type;
	} regs[] = {
		"AX", AREG_AX,
		"AL", AREG_AL,
		"AH", AREG_AH,
		"EAX", AREG_EAX,

		"BC", AREG_BC,
		"BL", AREG_BL,
		"BH", AREG_BH,
		"EBX", AREG_EBX,

		"CX", AREG_CX,
		"CL", AREG_CL,
		"CH", AREG_CH,
		"ECX", AREG_ECX,

		"DX", AREG_DX,
		"DL", AREG_DL,
		"DH", AREG_DH,
		"EDX", AREG_EDX,

		"SI", AREG_SI,
		"DI", AREG_DI,

		"SP", AREG_SP,
		"ESP", AREG_ESP,
		"EBP", AREG_EBP,
		NULL
	}, *bp;

	for (bp = regs; bp->name; ++bp) {
		Symbol *sym = lookup(bp->name);
		sym->argtype = bp->type;
	}
}

int
match(Op *op, Node **args)
{
	char *p;
	int a, olda;

	if (!op->args)
		return args == NULL;

	for (p = op->args; *p; ++p) {
		if (*p != AREP)
			a = *p;
		else
			--p;

		switch (a) {
		case AIMM8:
		case AIMM16:
		case AIMM32:
		case AIMM64:
		case AREG_AL:
		case AREG_AH:
		case AREG_AX:
		case AREG_EAX:
		default:
			abort();
		}
	}
	return 1;
}
