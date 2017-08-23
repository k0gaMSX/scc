# See LICENSE file for copyright and license details.
.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

include ../config.mk

MOREFLAGS = -I../inc/$(STD)
LIBDIR    = ../lib/cc

OBJ = types.o decl.o lex.o error.o symbol.o main.o expr.o \
      code.o stmt.o cpp.o fold.o init.o builtin.o

HDR = cc1.h ../inc/cc.h ../inc/$(STD)/cstd.h ../inc/sysincludes.h

all:
	mkdir -p ../rootdir/libexec/scc/
	cp cc1-* ../rootdir/libexec/scc/

cpp.o: stallman.msg

$(LIBDIR)/libcc.a:
	cd $(LIBDIR) && $(MAKE)

dep:
	./gendep.sh $(TARGETS)

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc1-*

distclean: clean
	rm -f makefile

include ../lib/cc/libdep.mk
