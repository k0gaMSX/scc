.POSIX:

LIBDIR  = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

OBJ = main.o symbol.o ins.o parser.o expr.o myro.o
HDR = ../inc/scc.h as.h
MOREFLAGS = -I../inc/$(STD) $(AS_CFLAGS)

all:

$(OBJ): $(HDR)

as: $(OBJ)
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

myro.o: ../inc/myro.h

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

dep:
	./gendep.sh $(TARGETS)

clean:
	rm -f *.o target/*/*.o
	rm -f target/*/*tbl.c
	rm -f as-*
	rm -f *.log

distclean: clean
	rm -f makefile

