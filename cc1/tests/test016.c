/* See LICENSE file for copyright and license details. */

/*
name: TEST016
description: Basic pointer test
error:
test016.c:42: error: redefinition of 'func2'
test016.c:46: error: incompatible types when assigning
output:
V2	K	#NC
V4	K	#N9
G5	I	"g
G7	I	F	"func1
{
\
A8	I	"x
A10	P	"p
	G5	#I1	:I
	A8	#I1	:I
	A10	A8	'P	:P
	A10	@I	#I0	:I
	y	L11	A8	#I0	=I
	h	#I1
L11
	A10	G5	'P	:P
	A10	@I	#I0	:I
	y	L12	A10	#P0	!I
	h	#I1
L12
	h	#I0
}
G13	I	F	"func2
{
\
A14	I	"x
A16	P	"p
A18	P	"pp
	A14	#I1	:I
	A16	A14	'P	:P
	A18	A16	'P	:P
	y	L19	A16	#P0	=I
	A18	@P	@I	#I0	:I
L19
	A16	#P0	:P
	h	A14
}
*/

#line 1

int g;

int
func1(void)
{
	int  x;
	int *p;

	g = 1;
	x = 1;
	p = &x;
	*p = 0;
	if (x)
		return 1;

	p = &g;
	*p = 0;
	if (p == 0)
		return 1;
	return 0;
}

int
func2(void)
{
	int   x;
	int  *p;
	int **pp;

	x = 1;
	p = &x;
	pp = &p;
	if (p != 0)
		**pp = 0;
	p = 0;
	return x;
}

int
func2(void)
{
	char c;
	int *p;

	p = &c;
	return *p;
}
