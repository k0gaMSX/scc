# See LICENSE file for copyright and license details.
.POSIX:

LIBDIR = ./
include ../../config.mk
include libdep.mk

all: libscc.a

libscc.a: $(LIB-OBJ)
	ar $(ARFLAGS) $@ $?
	ranlib $@

dep:

clean:
	rm -f *.o *.a

distclean: clean
