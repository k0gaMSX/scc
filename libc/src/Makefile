# See LICENSE file for copyright and license details.
.POSIX:

LIBCOBJ = assert.o strcpy.o strcmp.o

all: libc.a

libc.a: $(LIBCOJB)
	ar $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o libc.a
