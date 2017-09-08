
#include "../inc/scc.h"
#include "as.h"
#include "ins.h"

void
direct(Op *op, Arg *args)
{
	emit(cursec, op->bytes, op->size);
}
