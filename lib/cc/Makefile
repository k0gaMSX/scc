# See LICENSE file for copyright and license details.
.POSIX:

include ../../config.mk

LIBDIR = ./

all: libcc.a

libcc.a:
	ar $(ARFLAGS) $@ $?
	ranlib $@

dep:

clean:
	rm -f *.o *.a

distclean: clean
include libdep.mk
