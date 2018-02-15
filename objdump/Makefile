.POSIX:

LIBDIR    = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o 

all: objdump

objdump: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

main.o: ../inc/scc.h ../inc/myro.h ../inc/arg.h

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	+cd $(LIBDIR) && $(MAKE)

dep:
clean:
	rm -f objdump *.o

distclean: clean
