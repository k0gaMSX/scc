# See LICENSE file for copyright and license details.
.POSIX:

CSTDINC = -I../inc/$(STD)

include ../config.mk

OBJS = types.o decl.o lex.o error.o symbol.o main.o expr.o \
	code.o stmt.o cpp.o fold.o init.o arch/$(ARCH)/arch.o

all: cc1-$(ARCH)

cpp.o: stallman.msg
$(OBJS): cc1.h ../inc/cc.h ../inc/$(STD)/cstd.h

../lib/libcc.a:
	cd ../lib && $(MAKE) -e

cc1-$(ARCH): $(OBJS) ../lib/libcc.a
	$(CC) $(SCC_LDFLAGS) $(OBJS) ../lib/libcc.a -o $@

cpp: cc1-$(ARCH)
	ln -f $< cpp

test:
	cd tests && ./chktest.sh *.c

clean:
	rm -f $(OBJS)
	rm -f cc1-* cpp
