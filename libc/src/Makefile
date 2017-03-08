# See LICENSE file for copyright and license details.
.POSIX:

include ../../config.mk
include sys/$(ARCH)-$(SYS)/system.mk

LIBCOBJ = assert.o strcpy.o strcmp.o strlen.o strchr.o \
          strrchr.o strcat.o strncmp.o strncpy.o strncat.o strcoll.o \
          strxfrm.o strstr.o strspn.o strcspn.o strpbrk.o strtok.o \
          memset.o memcpy.o memmove.o memcmp.o memchr.o \
          isalnum.o isalpha.o isascii.o isblank.o iscntrl.o isdigit.o \
          isgraph.o islower.o isprint.o ispunct.o isspace.o isupper.o \
          isxdigit.o toupper.o tolower.o ctype.o setlocale.o \
          localeconv.o atoi.o atexit.o exit.o \
          printf.o fprintf.o vfprintf.o \
          realloc.o calloc.o malloc.o

OBJ = $(LIBCOBJ) $(SYSOBJ)

all: libc.a

$(SYSOJB):
	cd sys/$(ARCH)-$(SYS) && make all

libc.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o libc.a
	cd sys/$(ARCH)-$(SYS) && make clean
