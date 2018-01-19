
Z80_LST = target/x80/z80tbl.o target/x80/z80.o target/x80/ins.o

$(Z80_LST): target/x80/proc.h

target/x80/z80tbl.c: gentbl.awk target/x80/x80.dat target/x80/rules.dat
	./gentbl.sh -f x80 -c z80

OBJ-z80 = $(OBJ) $(Z80_LST)
