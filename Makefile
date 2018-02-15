# scc - Suckless C Compiler

include config.mk.def

.POSIX:

DIRS  = inc cc1 cc2 driver lib as ar nm objdump

all:
	$(FORALL)

clean:
	$(FORALL)
	rm -rf rootdir

distclean:
	touch config.mk    # we need config.mk for makes in $DIRS
	find . -name makefile | xargs rm -f
	$(FORALL)
	rm -rf rootdir
	rm -f config.mk

config.mk:
	trap "rm -f $$.mk" 0 2 3; \
	(cat config.mk.def ;\
	sed -n '/^# '`uname`'/,/^$$/p' system.mk) > $$.mk && \
	mv $$.mk config.mk

dep: config.mk
	$(FORALL)

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
