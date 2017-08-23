# See LICENSE file for copyright and license details.
.POSIX:

LIBDIR    = ../../lib/scc
include ../../config.mk
include $(LIBDIR)/libdep.mk

OBJ = scc.o
HDR = config.h \
      ../../inc/scc.h \
      ../../inc/arg.h \
      ../../inc/syslibs.h \
      ../../inc/ldflags.h

all: scc scpp
	mkdir -p ../../rootdir/bin
	cp scc scpp ../../rootdir/bin/

dep:
	PREFIX=$(PREFIX) USEQBE=$(USEQBE) ./gendep.sh $(TARGETS)

$(OBJ): $(HDR)

scc: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

scpp: cpp.sh config.h
	set -x ;\
	trap "rm -f $$$$.sh" 0 2 3;\
	rm -f $@ ;\
	sed "s%@PREFIX@%$(PREFIX)%" < cpp.sh > $$$$.sh && \
	chmod +x $$$$.sh && \
	mv $$$$.sh scpp

$(LIBDIR)/libcc.a:
	cd $(LIBDIR) && $(MAKE)

clean:
	rm -f $(OBJ)
	rm -f scc
	rm -f scpp

distclean: clean
	rm -f config.h

