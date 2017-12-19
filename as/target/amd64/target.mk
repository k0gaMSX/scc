
AMD64_LST = target/amd64/instbl.o target/amd64/ins.o target/amd64/proc.o

$(AMD64_LST): target/x86/proc.h

target/amd64/instbl.c: target/gen.awk target/x86/x86.dat target/x86/rules.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	awk '!/^$$/ {print $$1,NR,$$2,$$3,$$4,$$5,$$6}' target/x86/x86.dat | \
	sort -k1 -k2n | \
	awk -v cpu=BITS64 -v family=x86 \
		-f target/gen.awk  > $$$$.c && mv $$$$.c $@

OBJ-amd64 = $(OBJ) $(AMD64_LST)
