.POSIX:

# Makefile is only used as a template for makefile.
# If you modify Makefile remember to run make dep

LIBDIR    = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

MOREFLAGS = -I../inc/$(STD) $(CC2_CFLAGS)

OBJ = main.o parser.o peep.o symbol.o node.o code.o optm.o
HDR = cc2.h ../inc/$(STD)/cstd.h ../inc/scc.h

all:
	mkdir -p ../rootdir/libexec/scc/
	cp cc2-* ../rootdir/libexec/scc/

dep:
	MKQBE=${MKQBE} ./gendep.sh $(TARGETS)

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	cd $(LIBDIR) && $(MAKE) $(MFLAGS)

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
