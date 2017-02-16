
LIBCOBJ = assert.o

libc.a: $(LIBCOJB)
	ar $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o libc.a
