.POSIX:

include ../../../config.mk

OBJ = bsearch.o \
      abs.o __abs.o labs.o __labs.o llabs.o __labs.o \
      printf.o fprintf.o vfprintf.o \
      fgets.o gets.of fgetc.o fputc.o getchar.o putchar.o \
      fputs.o puts.o fread.o fwrite.o \
      getc.o putc.o __putc.o __getc.o \
      rewind.o fseek.o ferror.o feof.o clearerr.o \
      realloc.o calloc.o malloc.o \
      assert.o strcpy.o strcmp.o strlen.o strchr.o \
      strrchr.o strcat.o strncmp.o strncpy.o strncat.o strcoll.o \
      strxfrm.o strstr.o strspn.o strcspn.o strpbrk.o strtok.o \
      memset.o memcpy.o memmove.o memcmp.o memchr.o \
      isalnum.o isalpha.o isascii.o isblank.o iscntrl.o isdigit.o \
      isgraph.o islower.o isprint.o ispunct.o isspace.o isupper.o \
      isxdigit.o toupper.o tolower.o ctype.o setlocale.o \
      localeconv.o atoi.o atol.o atoll.o atexit.o abort.o exit.o

all: $(ARCH)-libc.a

clean distclean:
	rm -f *.o *-libc.a

$(ARCH)-libc.a: $(OBJ)
	$(AR) $(ARFLAGS) $@ $?
	ranlib $@
