# scc - Suckless C Compiler
.POSIX:

include config.mk

SUBDIRS  = lib cc1 cc2 driver/$(DRIVER)
ARCHS   = z80 i386-sysv amd64-sysv

all clean:
	for i in ${SUBDIRS};\
	do \
		(cd $$i; ${MAKE} -$(MAKEFLAGS) $@ || exit); \
	done

scc: lib/libcc.a
	cd driver/$(DRIVER) && make $@

multi: lib/libcc.a
	for i in $(ARCHS) ; \
	do \
		$(MAKE) -$(MAKEFLAGS) $$i || exit ;\
	done

lib/libcc.a:
	cd lib && $(MAKE) -$(MAKEFLAGS) all


$(ARCHS): lib/libcc.a
	for i in cc1 cc2; \
	do \
		(cd $$i; \
		 ARCH=$@ ${MAKE} -e -$(MAKEFLAGS) clean ;\
		 ARCH=$@ $(MAKE) -e $$i || exit); \
	done
	ln -f cc1/cc1 bin/cc1-$@
	ln -f cc2/cc2 bin/cc2-$@

install: scc
	mkdir -p $(PREFIX)/libexec/scc/
	mkdir -p $(PREFIX)/bin/
	cp -f bin/cc[12]-* $(PREFIX)/libexec/scc/
	cp -f driver/$(DRIVER)/scc $(PREFIX)/bin/
	cd $(PREFIX)/libexec/scc/ && ln -f cc1-$(ARCH) cc1
	cd $(PREFIX)/libexec/scc/ && ln -f cc2-$(ARCH) cc2
	cd $(PREFIX)/libexec/scc/ && ln -f cc1 cpp
	cd $(PREFIX)/libexec/scc/ && chmod 755 cc[12]-* cc1 cc2 cpp

uninstall:
	rm -rf $(PREFIX)/libexec/scc/
	rm -f $(PREFIX)/bin/scc


distclean: clean
	rm -f bin/cc[12]-*
