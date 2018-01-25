static char sccsid[] = "@(#) ./as/target/x86/ins.c";

#include <stdlib.h>

#include "../../../inc/scc.h"
#include "../../as.h"
#include "proc.h"

int
match(Op *op, Node **args)
{
	unsigned char *p;
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

Node *
moperand(void)
{
}
