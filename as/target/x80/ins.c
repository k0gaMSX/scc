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

int
ddclass(int reg)
{
	switch (reg) {
	case AREG_BC:
	case AREG_DE:
	case AREG_HL:
	case AREG_SP:
		return 1;
	default:
		return 0;
	}
}

int
qqclass(int reg)
{
	switch (reg) {
	case AREG_BC:
	case AREG_DE:
	case AREG_HL:
	case AREG_AF:
		return 1;
	default:
		return 0;
	}
}

int
ppclass(int reg)
{
	switch (reg) {
	case AREG_BC:
	case AREG_DE:
	case AREG_IX:
	case AREG_SP:
		return 1;
	default:
		return 0;
	}
}

int
rrclass(int reg)
{
	switch (reg) {
	case AREG_BC:
	case AREG_DE:
	case AREG_IY:
	case AREG_SP:
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
	case AREG_BC:  return 0;
	case AREG_DE:  return 1;
	case AREG_HL:
	case AREG_IX:
	case AREG_IY:  return 2;
	case AREG_AF:
	case AREG_SP:  return 3;
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
	emit(buf, n);
}

void
r8_idx(Op *op, Node **args)
{
	args[1] = args[1]->left;
	r8_imm8(op, args);
}

void
idx_r8(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0]->left;
	par2 = args[1];

	memcpy(buf, op->bytes, n-1);
	buf[n-1] = par1->sym->value;
	buf[n-2] |= reg2int(par2->sym->argtype);
	emit(buf, n);
}

void
idx_imm8(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0]->left;
	par2 = args[1];

	memcpy(buf, op->bytes, n-1);
	buf[n-1] = par1->sym->value;
	buf[n-2] = par2->sym->value;
	emit(buf, n);
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
	emit(buf, n);
}

void
imm16(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	unsigned val;
	int n = op->size;

	par2 = args[1];

	memcpy(buf, op->bytes, n-2);
	val = par2->sym->value;
	buf[n-1] = val >> 8;
	buf[n-2] = val;
	emit(buf, n);
}

void
dir(Op *op, Node **args)
{
	args[1] = args[1]->left;
	imm16(op, args);
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
	emit(buf, n);
}

void
xx_r8(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n);
	buf[n-1] |= reg2int(par2->sym->argtype);
	emit(buf, n);
}

void
r8_xx(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[3];
	int n = op->size;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n);
	buf[n-1] |= reg2int(par1->sym->argtype) << 3;
	emit(buf, n);
}

void
r16(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[4];
	int n = op->size;

	par1 = args[0];

	memcpy(buf, op->bytes, n-1);
	buf[n-1] |= reg2int(par1->sym->argtype) << 4;
	emit(buf, n);
}

void
r16_imm16(Op *op, Node **args)
{
	Node *par1, *par2;
	unsigned char buf[4];
	int n = op->size;
	unsigned val;

	par1 = args[0];
	par2 = args[1];

	memcpy(buf, op->bytes, n-1);
	val = par2->sym->value;
	buf[n-1] = val >> 8;
	buf[n-2] = val;
	buf[n-3] |= reg2int(par1->sym->argtype) << 4;
	emit(buf, n);
}

void
r16_dir(Op *op, Node **args)
{
	args[1] = args[1]->left;
	r16_imm16(op, args);
}

void
r8(Op *op, Node **args)
{
	/* TODO */
	abort();
}

void
idx(Op *op, Node **args)
{
	/* TODO */
	abort();
}

void
im(Op *op, Node **args)
{
	/* TODO */
	abort();
}
