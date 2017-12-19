
I386_LST = target/i386/instbl.o target/i386/ins.o target/i386/proc.o

$(I386_LST): target/x86/proc.h

target/i386/instbl.c: target/gen.awk target/x86/x86.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	awk '!/^$$/ {print $$1,NR,$$2,$$3,$$4,$$5,$$6}' target/x86/x86.dat | \
	sort -k1 -k2n | \
	awk -v cpu=BITS32 -v family=x86 \
		-f target/gen.awk > $$$$.c && mv $$$$.c $@

OBJ-i386 = $(OBJ) $(I386_LST)
