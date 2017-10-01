static char sccsid[] = "@(#) ./as/target/z80/ins.c";

#include <stdlib.h>
#include <string.h>

#include "../../../inc/scc.h"
#include "../../as.h"
#include "proc.h"

int
rclass(int reg)
{
	switch (reg) {
	case AREG_B:
	case AREG_C:
	case AREG_D:
	case AREG_E:
	case AREG_H:
	case AREG_L:
	case AREG_A:
		return 1;
	default:
		return 0;
	}
}

int
pclass(int reg)
{
	switch (reg) {
	case AREG_B:
	case AREG_C:
	case AREG_D:
	case AREG_E:
	case AREG_IXH:
	case AREG_IXL:
	case AREG_A:
		return 1;
	default:
		return 0;
	}
}

int
qclass(int reg)
{
	switch (reg) {
	case AREG_B:
	case AREG_C:
	case AREG_D:
	case AREG_E:
	case AREG_IYH:
	case AREG_IYL:
	case AREG_A:
		return 1;
	default:
		return 0;
	}
}

static int
reg2int(int reg)
{
	switch (reg) {
	case AREG_B:   return 0;
	case AREG_C:   return 1;
	case AREG_D:   return 2;
	case AREG_E:   return 3;
	case AREG_IXH:
	case AREG_IYH:
	case AREG_H:   return 4;
	case AREG_IXL:
	case AREG_IYL:
	case AREG_L:   return 5;
	case AREG_A:   return 7;
	default:       abort();
	}
}

void
r8_imm8(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n-1);
	buf[n-1] = par2->sym->value;
	buf[n-2] |= reg2int(par1->sym->argtype) << 3;
	emit(cursec, buf, n);
}

void
imm8(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par2 = args[1];

	memcpy(buf, op->bytes, n-1);
	buf[n-1] = par2->sym->value;
	emit(cursec, buf, n);
}

void
r8_r8(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];
	memcpy(buf, op->bytes, n);
	buf[n-1] |= reg2int(par1->sym->argtype) << 3 | 
	            reg2int(par2->sym->argtype);
	emit(cursec, buf, n);
}

void
xx_r8_2(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n);
	buf[n-1] |= reg2int(par2->sym->argtype);
	emit(cursec, buf, n);
}

void
r8_xx_2(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n);
	buf[n-1] |= reg2int(par2->sym->argtype) << 3;
	emit(cursec, buf, n);
}

void
r8_xx_1(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n);
	buf[n-1] |= reg2int(par1->sym->argtype) << 3;
	emit(cursec, buf, n);
}
