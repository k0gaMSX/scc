/* See LICENSE file for copyright and license details. */

/*
name: TEST034
description: Basic test for incomplete structures
error:
test034.c:50: error: declared variable 'bar' of incomplete type
test034.c:50: error: redeclaration of 'bar'
output:
V2	K	#NC
V4	K	#N9
X7	S6	"x
X9	I	F	"foo
G10	I	F	"main
{
\
X11	S6	"x
	h	X11	'P	#P0	!I
}
S6	"X	#N2	#N1
M13	I	"v	#N0
G9	I	F	"foo
{
\
	X7	M13	.I	#I0	:I
	h	X7	M13	.I
}
X17	S15	"bar2
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
