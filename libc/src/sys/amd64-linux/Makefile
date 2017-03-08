# See LICENSE file for copyright and license details.
.POSIX:

include ../../../../config.mk

OBJ_GEN = _read.o _write.o _open.o _close.o _lseek.o _brk.o\
          _rt_sigaction.o _getpid.o _Exit.o _kill.o

ASM_GEN = $(OBJ_GEN:.o=.s)
OBJ = $(OBJ_GEN)

all: $(OBJ)

.s.o:
	$(AS) $< -o $@

$(SRC):
	awk -f syscall.awk < syscall.lst

clean:
	rm -f *.s *.o
