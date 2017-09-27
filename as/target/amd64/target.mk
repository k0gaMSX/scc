
target/amd64/instbl.o: target/amd64/ins.h

target/amd64/instbl.c: target/gen.awk target/x86/x86.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	awk -v bits=BITS32 -v proc=x86 \
		-f target/gen.awk  \
		< target/x86/x86.dat > $$$$.c && mv $$$$.c $@

OBJ-amd64 = $(OBJ) \
	target/amd64/instbl.o \
	target/i386/ins.o \
	target/amd64/proc.o
