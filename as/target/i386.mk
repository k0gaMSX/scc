
i386_LST = target/x86/i386tbl.o target/x86/i386.o target/x86/ins.o

$(i386_LST): target/x86/proc.h

target/x86/i386tbl.c: gentbl.awk target/x86/x86.dat target/x86/rules.dat
	./gentbl.sh -f x86 -c i386

OBJ-i386 = $(OBJ) $(i386_LST)
