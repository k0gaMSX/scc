.POSIX:

include ../../../config.mk

all clean distclean: system.mk
	$(MAKE) $(MFLAGS) ABI=$(ABI) -f ../libsys.mk $@

system.mk: syscall.lst
	rm -f $@; trap "rm -f $$$$.mk" 0 2 3; \
	awk 'BEGIN{printf "OBJ = "} \
	     ! /^#/ {printf "%s.o ", $$2}' syscall.lst > $$$$.mk && \
	mv $$$$.mk $@
