.POSIX:

# fallback case if DRIVER isn't defined
DRIVER = posix

include ../config.mk

all dep clean distclean:
	+cd $(DRIVER) && $(MAKE) $@
