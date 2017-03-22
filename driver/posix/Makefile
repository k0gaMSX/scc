# See LICENSE file for copyright and license details.
.POSIX:

include ../../config.mk

OBJS = scc.o

all: scc

$(OBJS): ../../inc/cc.h ../../inc/arg.h ../../inc/syslibs.h ../../inc/ldflags.h

scc: $(OBJS) ../../lib/libcc.a
	$(CC) $(SCC_LDFLAGS) $(OBJS) ../../lib/libcc.a -o $@

../../inc/syslibs.h: ../../inc/syslibs.def.h
	cp -f ../../inc/syslibs.def.h ../../inc/syslibs.h

../../inc/ldflags.h: ../../inc/ldflags.def.h
	cp -f ../../inc/ldflags.def.h ../../inc/ldflags.h

../../lib/libcc.a:
	cd ../../lib && $(MAKE) -e

clean:
	rm -f $(OBJS)
	rm -f scc
