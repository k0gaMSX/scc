
/*
name: TEST034
description: Basic test for incomplete structures
output:
test034.c:45: error: declared variable 'bar' of incomplete type
X3	S2	x
F4	I	E
X5	F4	foo
G6	F4	main
{
\
X7	S2	x
	r	X7	'P	#P0	!I
}
G5	F4	foo
{
\
	X3	M9	.I	#I0	:I
	r	X3	M9	.I
}
X13	S11	bar2
test034.c:45: error: redeclaration of 'bar'
*/

extern struct X x;
int foo();

int main()
{
	extern struct X x;
	return &x != 0;
}

struct X {int v;};

int foo()
{
	x.v = 0;
	return x.v;
}

typedef struct bar bar;
extern bar bar2;
bar bar;
