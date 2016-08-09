/* See LICENSE file for copyright and license details. */

/*
name: TEST034
description: Basic test for incomplete types
error:
test034.c:59: error: declared variable 'bar' of incomplete type
test034.c:59: error: redeclaration of 'bar'
output:
G2	V1	"v
V3	I	#N10
G2	V3	"v
Y4	V1	"p
Y4	V3	"p
X7	S6	"x
X9	I	F	"foo
G10	I	F	"main
{
\
X11	S6	"x
	h	X11	'P	#P0	!I
}
G9	I	F	"foo
{
\
	X7	M13	.I	#I0	:I
	h	X7	M13	.I
}
X17	S15	"bar2
*/

int v[];
int v[10];
static int p[];
static int p[10];
static int p[10];
int v[10];
int v[];

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
