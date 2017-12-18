
AMD64_LST = target/amd64/instbl.o target/amd64/ins.o target/amd64/proc.o

$(AMD64_LST): target/x86/proc.h

target/amd64/instbl.c: target/gen.awk target/x86/x86.dat target/x86/rules.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	sort -k1 target/x86/x86.dat | \
	awk -v cpu=BITS64 -v family=x86 \
		-f target/gen.awk  > $$$$.c && mv $$$$.c $@

OBJ-amd64 = $(OBJ) $(AMD64_LST)
