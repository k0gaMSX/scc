.POSIX:

LIBDIR    = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o

all: nm

nm: $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

main.o: ../inc/scc.h ../inc/ar.h ../inc/myro.h

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	cd $(LIBDIR) && $(MAKE)

dep:
clean:
	rm -f nm *.o

distclean: clean
