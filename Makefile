# scc - Suckless C Compiler
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

DIRS  = inc cc1 cc2 driver lib

all:
	@set -e ;\
	trap 'rm -f bin/cc1-* bin/cc2-* bin/scc' 0 2 3; \
	if ! test -f bin/dep ;\
	then \
		echo Run make dep first >&2 ;\
		exit 1 ;\
	fi ;\
	pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done
	cp cc1/cc1-* bin/
	cp cc2/cc2-* bin/
	cp driver/$(DRIVER)/scc bin/


clean:
	@set -e ;\
	pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done
	rm -f bin/cc1-*
	rm -f bin/cc2-*
	rm -f bin/scc

distclean:
	@set -e ;\
	pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done
	rm -f bin/cc1-*
	rm -f bin/cc2-*
	rm -f bin/scc
	rm -f bin/dep

dep:
	@set -e;\
	pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done
	touch bin/dep

tests: all
	cd tests && $(MAKE) -e all

install: all
	mkdir -p $(DESTDIR)/$(PREFIX)/libexec/scc/
	mkdir -p $(DESTDIR)/$(PREFIX)/bin/
	mkdir -p $(DESTDIR)/$(PREFIX)/include/scc/
	mkdir -p $(DESTDIR)/$(PREFIX)/lib/scc/
	cp -f bin/cc?-* $(DESTDIR)/$(PREFIX)/libexec/scc/
	cp -f bin/cpp.sh $(DESTDIR)/$(PREFIX)/bin/scpp
	cp -f bin/scc $(DESTDIR)/$(PREFIX)/bin/
	cp -f crt/crt-*.o $(DESTDIR)/$(PREFIX)/lib/scc/
	cp -fr libc/include/* $(DESTDIR)/$(PREFIX)/include/scc/
	find $(DESTDIR)/$(PREFIX)/include/scc/ -type f | xargs chmod 644
	cd $(DESTDIR)/$(PREFIX)/libexec/scc/ && chmod 755 cc* && strip cc*
	cd $(DESTDIR)/$(PREFIX)/bin && chmod 755 scpp scc && strip scc

uninstall:
	rm -rf $(DESTDIR)/$(PREFIX)/include/scc/
	rm -rf $(DESTDIR)/$(PREFIX)/libexec/scc/
	rm -rf $(DESTDIR)/$(PREFIX)/lib/scc/
	rm -f $(DESTDIR)/$(PREFIX)/bin/scc
	rm -f $(DESTDIR)/$(PREFIX)/bin/scpp
