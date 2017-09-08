
target/i386/ins.o: target/i386/ins.h

target/i386/instbl.c: target/x86/gen.awk target/x86/i386.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	awk -f target/x86/gen.awk < target/x86/i386.dat > $$$$.c && mv $$$$.c $@

OBJ-i386 = $(OBJ) \
	target/i386/instbl.o \
	target/i386/ins.o
