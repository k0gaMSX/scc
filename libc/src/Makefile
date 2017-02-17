# See LICENSE file for copyright and license details.
.POSIX:

LIBCOBJ = assert.o strcpy.o strcmp.o strlen.o strchr.o \
          strrchr.o strcat.o strncpy.o strncat.o strcoll.o \
          memset.o memcpy.o memmove.o memcmp.o memchr.o \
          isalnum.o isalpha.o isascii.o iscntrl.o isdigit.o isgraph.o \
          islower.o isprint.o ispunct.o isspace.o isupper.o isxdigit.o \
          toupper.o tolower.o \
          setlocale.o

all: libc.a

libc.a: $(LIBCOBJ)
	ar $(ARFLAGS) $@ $?
	ranlib $@

clean:
	rm -f *.o libc.a
