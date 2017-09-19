.POSIX:

LIBDIR  = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o emit.o ins.o parser.o expr.o
HDR = ../inc/scc.h as.h
MOREFLAGS = $(AS_CFLAGS)

all:

$(OBJ): $(HDR)

as: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	cd $(LIBDIR) && $(MAKE)

dep: ../config.mk Makefile
	./gendep.sh $(TARGETS)
	touch dep

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f target/*/instbl.c
	rm -f as-*

distclean: clean
	rm -f makefile dep

