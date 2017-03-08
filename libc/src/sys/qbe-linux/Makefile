# See LICENSE file for copyright and license details.
.POSIX:

include ../../../../config.mk

OBJ_GEN = _read.o _write.o _open.o _close.o _lseek.o _brk.o\
          _rt_sigaction.o _getpid.o _Exit.o _kill.o

ASM_GEN = $(OBJ_GEN:.o=.s)
OBJ = $(OBJ_GEN)

all: $(OBJ) system.mk

.s.o:
	$(AS) $< -o $@

system.mk: Makefile
	echo '$$(SYSOBJ)' = $(OBJ) > system.mk

$(ASM_GEN): syscall.lst
	awk -f syscall.awk < syscall.lst

clean:
	rm -f *.o
	rm -f $(ASM_GEN)
