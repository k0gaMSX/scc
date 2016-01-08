/*
name: TEST041
description: Test for bug parsing ternary operators
output:
test041.c:36: error: type mismatch in conditional expression
F1	I
G2	F1	main
{
\
A3	I	i
A4	P	p
S6	foo
M7	I	i
A8	S6	foo
	A3	A3	#I0	!I	#W0	#W0	?W	gI	:I
	A4	A3	#I0	!I	#P0	#P0	?P	:P
	A4	A3	#I0	!I	#P0	#P0	?P	:P
	A4	A3	#I0	!I	#P0	#P0	?P	:P
test041.c:36: error: incompatible types when assigning
test041.c:37: error: used struct/union type value where scalar is required
test041.c:38: warning: 'i' defined but not used
test041.c:38: warning: 'foo' defined but not used
*/

int
main(void)
{
	int i;
	void *p;
	struct foo {int i;} foo;

	i = i ? 0 : 0l;
	p = i ? (void *) 0 : 0;
	p = i ? 0 : (void *) 0;
	p = i ? 0 : (const void *) 0;
	p = i ? 2 : p;
	foo ? 1 : 2;
}

