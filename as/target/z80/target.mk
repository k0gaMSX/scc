
Z80_LST = target/z80/instbl.o target/z80/ins.o target/z80/proc.o target/x80/ins.o

$(Z80_LST): target/x80/proc.h

target/z80/instbl.c: target/gen.awk target/x80/x80.dat
	set -e ;\
	rm -f $@;\
	trap "rm -f $$$$.c" 0 2 3; \
	awk -v cpu=Z80 -v family=x80 \
		-f target/gen.awk  \
		< target/x80/x80.dat > $$$$.c && mv $$$$.c $@

OBJ-z80 = $(OBJ) $(Z80_LST)
