static char sccsid[] = "@(#) ./as/ins.c";

#include "../inc/scc.h"
#include "as.h"

void
direct(Op *op, Arg *args)
{
	emit(cursec, op->bytes, op->size);
}

void
def(Arg *args, int siz)
{
	for ( ; args->type; ++args)
		emit(cursec, pack(args->val, siz, endian), siz);
}

void
defb(Op *op, Arg *args)
{
	def(args, 1);
}

void
defw(Op *op, Arg *args)
{
	def(args, 2);
}

void
defd(Op *op, Arg *args)
{
	def(args, 4);
}

void
defq(Op *op, Arg *args)
{
	def(args, 8);
}

void
equ(Op *op, Arg *args)
{
	if (!linesym)
		error("label definition lacks a label");
	else
		linesym->value = args->val;
}
