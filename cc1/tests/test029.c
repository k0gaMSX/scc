/* See LICENSE file for copyright and license details. */

/*
name: TEST029
description: Test of nested expansion and refusing macro without arguments
comments: f(2) will expand to 2*g, which will expand to 2*f, and in this
          moment f will not be expanded because the macro definition is
          a function alike macro, and in this case there is no arguments.
error:
test029.c:34: warning: expression without side effects
test029.c:39: error: redefinition of 'f1'
test029.c:40: error: 'f' undeclared
test029.c:40: warning: expression without side effects
output:
V2	K	#NC
V4	K	#N9
G7	I	F	"f1
{
\
A8	I	"f
	A8	#I2	*I
}
*/


#define f(a) a*g
#define g f

int
f1(void)
{
	int f;

	f(2);
}

int
f1(void)
{
	f(2);
}
