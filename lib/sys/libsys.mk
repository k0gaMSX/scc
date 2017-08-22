# See LICENSE file for copyright and license details.
.POSIX:

include ../../../config.mk
include system.mk

ASM = $(OBJ:.o=.s)

all: libsys.a

libsys.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

$(ASM): syscall.lst
	../abi/$(ABI).sh

clean:
	rm -f $(ASM)
	rm -f $(OBJ)
	rm -f libsys.a

distclean: clean
	rm -f system.mk
