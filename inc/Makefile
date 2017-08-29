.POSIX:

include ../config.mk

HDR = ldflags.h sysincludes.h syslibs.h

all:

dep: $(HDR)

ldflags.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	cat < ldflags.def.h > $$$$.h; \
	mv $$$$.h $@

sysincludes.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < sysincludes.def.h > $$$$.h && \
	mv $$$$.h $@

syslibs.h:
	rm -f $@ ; \
	trap "rm -f $$$$.h" 0 2 3; \
	sed 's,PREFIX,"$(PREFIX)",g' < syslibs.def.h > $$$$.h && \
	mv $$$$.h $@

clean:
distclean:
	rm -f $(HDR)
