# See LICENSE file for copyright and license details.

SRC=$(OBJ:.o=.c)

all: libc.a

libc.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

dep:
	../gendep.sh

clean:
	rm -f $(OBJ)

distclean: clean
	rm -f $(SRC)
	rm -f makefile
