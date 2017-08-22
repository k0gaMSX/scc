# scc version
VERSION = 0.1

## Customize below to fit your system
# TARGET is defined by backend-arch-abi-sys

TARGETS = z80-scc-dos-com \
          amd64-sysv-linux-elf \
          i386-sysv-linux-elf \
          amd64-sysv-openbsd-elf

# USEQBE selects QBE by default in the targets that support it
USEQBE = 1

#MKQBE enable build of 
MKQBE = 1

TARGET  = amd64-sys-linux-elf

DRIVER = posix

# Can be c89 or c99
STD = c99

# paths
PREFIX    = $(HOME)
MANPREFIX = $(PREFIX)/share/man

# scc expects to be built by a C99 compiler
# if your system is not at least POSIX 2004 compatible, adjust CC
# CC = c99
# AR = ar
AS = as

# for Plan9 add -D_SUSV2_SOURCE
SCC_CFLAGS = $(MOREFLAGS) \
             -g \
             $(CFLAGS)

SCC_LDFLAGS = -L$(LIBDIR)/ $(LDFLAGS)

.s.o:
	$(AS) $< -o $@

.c.o:
	$(CC) $(SCC_CFLAGS) -o $@ -c $<

.c:
	$(CC) $(SCC_CFLAGS) $(SCC_LDFLAGS) -o $@ $<

