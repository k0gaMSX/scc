
#include "../../../inc/scc.h"
#include "../../as.h"
#include "ins.h"

TUINT maxaddr = ((TUINT) 1 << 32) -1;

void
direct(Op *op, Arg *args)
{
	emit(cursec, op->bytes, op->size);
}
