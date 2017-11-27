.POSIX:

LIBDIR = ./
include ../../config.mk
include libdep.mk

all: libscc.a

$(LIB-OBJ): ../../inc/scc.h
$(LIBDIR)/wmyro.o: ../../inc/myro.h
$(LIBDIR)/rmyro.o: ../../inc/myro.h

libscc.a: $(LIB-OBJ)
	ar $(ARFLAGS) $@ $?
	ranlib $@

dep:

clean:
	rm -f *.o *.a

distclean: clean
