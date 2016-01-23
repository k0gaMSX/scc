# scc - Suckless C Compiler
.POSIX:

include config.mk

SUBDIRS  = lib cc1 cc2

ARCHS   = z80 i386-sysv amd64-sysv

all clean:
	for i in ${SUBDIRS};\
	do \
		(cd $$i; ${MAKE} -$(MAKEFLAGS) $@ || exit); \
	done

multi: $(ARCHS)

lib/libcc.a:
	cd lib && $(MAKE) -$(MAKEFLAGS) all

$(ARCHS): lib/libcc.a
	for i in cc1 cc2; \
	do \
		(cd $$i; ARCH=$@ ${MAKE} -e $(MAKEFLAGS) clean $$i || exit); \
	done
	ln -f cc1/cc1 bin/cc1-$@
	ln -f cc2/cc2 bin/cc2-$@

install:
	mkdir -p $(DESTDIR)$(PREFIX)/libexec/scc/
	cp -f bin/cc[12]-*
	cd $(DESTDIR)$(PREFIX)/libexec/scc/ && chmod 755 cc[12]-*


distclean: clean
	rm -f bin/cc[12]-*
