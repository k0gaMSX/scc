# scc version
VERSION     = 0.1

# Customize below to fit your system
ARCH = z80

# paths
PREFIX    = /usr/local/
MANPREFIX = ${PREFIX}/share/man

# if your system is not POSIX maybe you want to use cc or gcc
CC = c99
LD = $(CC)
AR = ar

# for Plan9 add -D_SUSV2_SOURCE -DNBOOL
CFLAGS   = -DNDEBUG -Iarch/$(ARCH) -DPREFIX=\""$(PREFIX)"\"
LDFLAGS  = -s
