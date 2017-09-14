static char sccsid[] = "@(#) ./as/target/i386/ins.c";

#include "../../../inc/scc.h"
#include "../../as.h"
#include "ins.h"

TUINT maxaddr = 0xFFFFFFFF;
int endian = LITTLE_ENDIAN;
