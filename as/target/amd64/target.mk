AMD64_TBL = common.dat \
            target/x86/i386.dat \
            target/x86/amd64.dat

target/amd64/instbl.o: target/amd64/ins.h

target/amd64/instbl.c: target/x86/gen.awk $(AMD64_TBL)
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	cat $(AMD64_TBL) |\
	awk -f target/x86/gen.awk > $$$$.c && mv $$$$.c $@

OBJ-amd64 = $(OBJ) \
	target/amd64/instbl.o \
	target/i386/ins.o \
	target/amd64/proc.o
