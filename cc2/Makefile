# See LICENSE file for copyright and license details.
.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

include ../config.mk

MOREFLAGS = -I../inc/$(STD)
LIBDIR    = ../lib/cc

OBJ = main.o parser.o peep.o symbol.o node.o code.o optm.o
HDR = cc2.h ../inc/$(STD)/cstd.h ../inc/cc.h

all:
	mkdir -p ../rootdir/libexec/scc/
	cp cc2-* ../rootdir/libexec/scc/

dep:
	MKQBE=${MKQBE} ./gendep.sh $(TARGETS)

$(LIBDIR)/libcc.a:
	cd $(LIBDIR) && $(MAKE)

main.o: error.h

error.h: cc2.h
	rm -f $@; trap 'rm -f $$$$.h' EXIT INT QUIT ;\
	awk -f generror.awk cc2.h > $$$$.h && mv $$$$.h $@

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc2-* error.h

distclean: clean
	rm -f makefile

include ../lib/cc/libdep.mk
