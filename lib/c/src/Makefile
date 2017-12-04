.POSIX:

include ../../../config.mk

OBJ = assert.o strcpy.o strcmp.o strlen.o strchr.o \
      strrchr.o strcat.o strncmp.o strncpy.o strncat.o strcoll.o \
      strxfrm.o strstr.o strspn.o strcspn.o strpbrk.o strtok.o \
      memset.o memcpy.o memmove.o memcmp.o memchr.o \
      isalnum.o isalpha.o isascii.o isblank.o iscntrl.o isdigit.o \
      isgraph.o islower.o isprint.o ispunct.o isspace.o isupper.o \
      isxdigit.o toupper.o tolower.o ctype.o setlocale.o \
      localeconv.o atoi.o atol.o atoll.o atexit.o exit.o \
      printf.o fprintf.o vfprintf.o \
      fgets.o gets.of fgetc.o fputc.o getchar.o putchar.o \
      realloc.o calloc.o malloc.o

all: $(ARCH)-libc.a

clean distclean:
	rm -f *.o *-libc.a

$(ARCH)-libc.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@
