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

all: scc cpp
	mkdir -p ../../rootdir/bin
	cp scc cpp ../../rootdir/bin/

dep:
	PREFIX=$(PREFIX) USEQBE=$(USEQBE) ./gendep.sh $(TARGETS)

$(OBJ): $(HDR)

scc: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

cpp: cpp.sh config.h
	set -x ;\
	trap "rm -f $$$$.sh" 0 2 3;\
	rm -f $@ ;\
	sed "s%@PREFIX@%$(PREFIX)%" < cpp.sh > $$$$.sh && \
	chmod +x $$$$.sh && \
	mv $$$$.sh $@

$(LIBDIR)/libscc.a:
	+cd $(LIBDIR) && $(MAKE)

clean:
	rm -f $(OBJ)
	rm -f scc
	rm -f scpp

distclean: clean
	rm -f config.h

