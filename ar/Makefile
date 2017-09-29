.POSIX:

LIBDIR    = ../lib/scc
include ../config.mk
include $(LIBDIR)/libdep.mk

OBJ       = main.o $(DRIVER)/stat.c
MOREFLAGS = -I$(DRIVER)

all: ar-$(DRIVER)

ar-$(DRIVER): $(OBJ) $(LIBDIR)/libscc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lscc -o $@

$(LIBDIR)/libscc.a: $(LIB-OBJ)
	cd $(LIBDIR) && $(MAKE)

main.o: $(DRIVER)/stat.h

dep:
clean:
	rm -f ar *.o $(DRIVER)/*.o

distclean: clean
