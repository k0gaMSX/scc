static char sccsid[] = "@(#) ./as/target/z80/ins.c";

#include "../../../inc/scc.h"
#include "../../as.h"
#include "proc.h"

int
r8(int reg)
{
	switch (reg) {
	case AREG_B:  return 0;
	case AREG_C:  return 1;
	case AREG_D:  return 2;
	case AREG_E:  return 3;
	case AREG_H:  return 4;
	case AREG_L:  return 5;
	case AREG_A:  return 7;
	default:      return -1;
	}
}

void
r8_imm(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[2];

	par1 = *args++;
	par2 = *args;
	buf[0] = *op->bytes | r8(par1->sym->argtype) << 3;
	buf[1] = par2->sym->value;
	emit(cursec, buf, 2);
}
