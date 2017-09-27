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
		"AF_", AREG_AF_,

		NULL,
	};
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
		default:
			abort();
		}
	}
	return 1;
}
