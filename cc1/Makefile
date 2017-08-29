.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

LIBDIR    = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

MOREFLAGS = -I../inc/$(STD)

OBJ = types.o decl.o lex.o error.o symbol.o main.o expr.o \
      code.o stmt.o cpp.o fold.o init.o builtin.o

HDR = cc1.h ../inc/scc.h ../inc/$(STD)/cstd.h ../inc/sysincludes.h

all:
	mkdir -p ../rootdir/libexec/scc/
	cp cc1-* ../rootdir/libexec/scc/

cpp.o: stallman.msg

$(LIBDIR)/libscc.a:
	cd $(LIBDIR) && $(MAKE)

dep:
	./gendep.sh $(TARGETS)

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f cc1-*

distclean: clean
	rm -f makefile

