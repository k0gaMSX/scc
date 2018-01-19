
amd64_LST = target/x86/amd64tbl.o target/x86/amd64.o target/x86/ins.o

$(amd64_LST): target/x86/proc.h

target/x86/amd64tbl.c: gentbl.awk target/x86/x86.dat target/x86/rules.dat
	./gentbl.sh -f x86 -c amd64

OBJ-amd64 = $(OBJ) $(amd64_LST)
