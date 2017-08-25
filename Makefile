# scc - Suckless C Compiler
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

DIRS  = inc cc1 cc2 driver lib

FORALL = @set -e ;\
	pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done

all: dep
	$(FORALL)

clean:
	$(FORALL)
	rm -rf rootdir

distclean:
	$(FORALL)
	rm -f dep
	rm -rf rootdir

dep: config.mk
	$(FORALL)
	touch dep

tests: all
	cd tests && $(MAKE) -e all

install: all
	cp -r rootdir/* $(DESTDIR)/$(PREFIX)/
	cd $(DESTDIR)/$(PREFIX)/libexec/scc/ && chmod 755 cc* && strip cc*
	cd $(DESTDIR)/$(PREFIX)/bin && chmod 755 scpp scc && strip scc

uninstall:
	rm -rf $(DESTDIR)/$(PREFIX)/libexec/scc/
	rm -rf $(DESTDIR)/$(PREFIX)/lib/scc/
	rm -f $(DESTDIR)/$(PREFIX)/bin/scc
	rm -f $(DESTDIR)/$(PREFIX)/bin/scpp
