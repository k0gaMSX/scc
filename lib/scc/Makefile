.POSIX:

LIBDIR = ./
include ../../config.mk
include libdep.mk

all: libscc.a

$(LIB-OBJ): ../../inc/scc.h

libscc.a: $(LIB-OBJ)
	ar $(ARFLAGS) $@ $?
	ranlib $@

dep:

clean:
	rm -f *.o *.a

distclean: clean
