# See LICENSE file for copyright and license details.
.POSIX:

LIBCOBJ = assert.o strcpy.o strcmp.o strlen.o strchr.o \
          strrchr.o strcat.o

all: libc.a

libc.a: $(LIBCOBJ)
	ar $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o libc.a
