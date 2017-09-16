# scc version
VERSION = 0.1

## Customize below to fit your system
# TARGETS is defined by a list of backend-arch-abi-sys. First
# element of the list becomes the default target

TARGETS = amd64-sysv-linux-elf \
          i386-sysv-linux-elf \
          amd64-sysv-openbsd-elf

# USEQBE selects QBE by default in the targets that support it
USEQBE = 1

#MKQBE enable build of qbe backends
MKQBE = 1

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
# for NetBSD add -D_ANSI_SOURCE
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

