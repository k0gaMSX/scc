# scc - Suckless C Compiler
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

DIRS  = lib cc1 cc2 driver/$(DRIVER)

all:
	for i in $(ARCHS); \
	do \
		$(MAKE) $$i || exit; \
	done

$(ARCHS):
	for i in cc1 cc2; \
	do \
		(cd $$i; \
		ARCH=$@ $(MAKE) -e $$i || exit); \
	done
	ln -f cc1/cc1 bin/cc1-$@
	ln -f cc2/cc2 bin/cc2-$@

install: all
	mkdir -p $(PREFIX)/libexec/scc/
	mkdir -p $(PREFIX)/bin/
	mkdir -p $(PREFIX)/include/scc
	cp -f bin/cc* $(PREFIX)/libexec/scc/
	cp -f bin/cc1 $(PREFIX)/bin/cpp
	cp -f bin/scc $(PREFIX)/bin/
	cp -fr libc/include/* $(PREFIX)/include/scc/
	find $(PREFIX)/include/scc/ -type f | xargs chmod 644
	cd $(PREFIX)/libexec/scc/ && chmod 755 cc* && strip cc*
	cd $(PREFIX)/bin && chmod 755 cpp scc && strip cpp scc

uninstall:
	rm -rf $(PREFIX)/libexec/scc/
	rm -f $(PREFIX)/bin/scc
	rm -f $(PREFIX)/bin/cpp

clean-helper:
	for i in $(DIRS); \
	do \
		(cd $$i && $(MAKE) clean || exit); \
	done

clean:
	for i in $(ARCHS); \
	do \
		ARCH=$$i $(MAKE) -e clean-helper || exit; \
	done
	rm -f bin/cc* bin/scc

distclean: clean
	rm -rf bin
	rm inc/sizes.h
