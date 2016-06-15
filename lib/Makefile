# See LICENSE file for copyright and license details.
.POSIX:
include ../config.mk

OBJS = die.o xcalloc.o xmalloc.o xrealloc.o xstrdup.o debug.o

all: libcc.a


libcc.a: $(OBJS)
	ar r $@ $?
	ranlib $@

clean:
	rm -f *.o *.a
