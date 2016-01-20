
/*
name: TEST028
description: Test of reinterpretation in define
error:
output:
F5	P
G6	F5	foo
{
\
	r	"686900	'P
}
*/


#define M(x) x
#define A(a,b) a(b)

char *
foo(void)
{
	return A(M,"hi");
}
