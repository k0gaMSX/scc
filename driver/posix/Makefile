# See LICENSE file for copyright and license details.
.POSIX:

include ../../config.mk

LIBDIR    = ../../lib/cc

OBJ = scc.o
HDR = config.h \
      ../../inc/cc.h \
      ../../inc/arg.h \
      ../../inc/syslibs.h \
      ../../inc/ldflags.h

all: scc

dep:
	PREFIX=$(PREFIX) USEQBE=$(USEQBE) ./gendep.sh $(TARGET)

$(OBJ): $(HDR)

scc: $(OBJ) $(LIBDIR)/libcc.a
	$(CC) $(SCC_LDFLAGS) $(OBJ) -lcc -o $@

$(LIBDIR)/libcc.a:
	cd $(LIBDIR) && $(MAKE)

clean:
	rm -f $(OBJ)
	rm -f scc

distclean: clean
	rm -f config.h

include ../../lib/cc/libdep.mk
