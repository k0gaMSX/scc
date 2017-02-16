# See LICENSE file for copyright and license details.
.POSIX:

LIBCOBJ = assert.o

libc.a: $(LIBCOJB)
	ar $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o libc.a
