# See LICENSE file for copyright and license details.
.POSIX:

include ../../../../config.mk

OBJ = _brk.o _close.o _exit.o _lseek.o _open.o _read.o _write.o
SRC = $(OBJ:.o=.s)

all: $(OBJ)

.s.o:
	$(AS) $< -o $@

$(SRC):
	awk -f syscall.awk < syscall.lst

clean:
	rm -f *.s *.o
