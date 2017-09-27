
target/i386/ins.o: target/i386/ins.h
target/i386/instbl.o: target/i386/ins.h

target/i386/instbl.c: target/gen.awk target/x86/x86.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	awk -v bits=BITS32 -v proc=x86 \
		-f target/gen.awk  \
		< target/x86/x86.dat > $$$$.c && mv $$$$.c $@

OBJ-i386 = $(OBJ) \
	target/i386/instbl.o \
	target/i386/ins.o \
	target/i386/proc.o
