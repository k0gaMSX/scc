
#include "../inc/scc.h"
#include "as.h"
#include "ins.h"

void
direct(Op *op, Arg *args)
{
	emit(cursec, op->bytes, op->size);
}

char *
pack(TUINT v, int n, int inc)
{
	static char buf[sizeof(TUINT)];
	int idx;

	idx = (inc < 0) ? n-1 : 0;
	while (n--) {
		buf[idx] = v;
		idx += inc;
		v >>= 8;
	}
	return buf;
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
