static char sccsid[] = "@(#) ./as/ins.c";

#include "../inc/scc.h"
#include "as.h"

void
direct(Op *op, Node **args)
{
	emit(cursec, op->bytes, op->size);
}

void
def(Node **args, int siz)
{
	Node *np;

	for ( ; np = *args; ++args)
		emit(cursec, pack(np->sym->value, siz, endian), siz);
}

void
defb(Op *op, Node **args)
{
	def(args, 1);
}

void
defw(Op *op, Node **args)
{
	def(args, 2);
}

void
defd(Op *op, Node **args)
{
	def(args, 4);
}

void
defq(Op *op, Node **args)
{
	def(args, 8);
}

void
equ(Op *op, Node **args)
{
	if (!linesym)
		error("label definition lacks a label");
	else
		linesym->value = (*args)->sym->value;
}
