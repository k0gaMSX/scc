.POSIX:

LIBDIR  = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o emit.o ins.o parser.o
HDR = ../inc/scc.h as.h

all:

$(OBJ): $(HDR)

as: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	cd $(LIBDIR) && $(MAKE)

dep:
	./gendep.sh $(TARGETS)

clean:
	rm -f *.o
	rm -f target/*/*.o
	rm -f target/*/instbl.c
	rm -f as-*

distclean: clean
	rm -f makefile

