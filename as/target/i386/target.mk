I386_TBL = common.dat \
           target/x86/i386.dat

target/i386/ins.o: target/i386/ins.h
target/i386/instbl.o: target/i386/ins.h

target/i386/instbl.c: target/x86/gen.awk $(I386_TBL)
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	cat $(I386_TBL) | \
	awk -f target/x86/gen.awk > $$$$.c && mv $$$$.c $@

OBJ-i386 = $(OBJ) \
	target/i386/instbl.o \
	target/i386/ins.o
