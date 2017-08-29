.POSIX:

include ../../config.mk

all clean distclean:
	pwd=$$PWD ;\
	for i in $(SYSS); \
	do \
		cd $$i; \
		$(MAKE) -e SYS=$i $@ ;\
		cd $$pwd; \
	done
