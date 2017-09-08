# scc - Suckless C Compiler
.POSIX:

include config.mk

DIRS  = inc cc1 cc2 driver lib as

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
	cp -r rootdir/* $(DESTDIR)$(PREFIX)/
	cd $(DESTDIR)$(PREFIX)/libexec/scc/ && chmod 755 cc* && strip cc*
	cd $(DESTDIR)$(PREFIX)/bin && chmod 755 scpp scc && strip scc
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < doc/scc.1 > $(DESTDIR)$(MANPREFIX)/man1/scc.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/scc.1

uninstall:
	rm -rf $(DESTDIR)$(PREFIX)/libexec/scc/
	rm -rf $(DESTDIR)$(PREFIX)/lib/scc/
	rm -f $(DESTDIR)$(PREFIX)/bin/scc
	rm -f $(DESTDIR)$(PREFIX)/bin/scpp
	rm -f $(DESTDIR)$(MANPREFIX)/man1/scc.1
