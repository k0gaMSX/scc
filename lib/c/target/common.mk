
all: libc.a

libc.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:
	gendep.sh

clean:
	rm -f *.o

distclean: clean
	rm -f makefile
