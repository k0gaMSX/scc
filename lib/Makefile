.POSIX:

include ../config.mk

DIRS = scc

all dep clean distclean:
	@pwd=$$PWD; \
	for i in $(DIRS); \
	do \
		cd $$i; \
		$(MAKE) $@; \
		cd $$pwd; \
	done
