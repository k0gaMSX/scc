/* See LICENSE file for copyright and license details. */

/*
name: TEST048
description: Basic test for initializer
error:
output:
V2	K	#NC
V4	K	#N9
S6	"S	#N4	#N1
M7	I	"a	#N0
M8	I	"b	#N2
V9	S6	#N1
G10	V9	"x	(
	#I1
	#I2
)
G12	I	F	"main
{
\
	y	L13	G10	M7	.I	#I1	=I
	h	#I1
L13
	y	L15	G10	M8	.I	#I2	=I
	h	#I2
L15
	h	#I0
}
*/

struct S {
	int a;
	int b;
};

struct S x[1] = {{1, 2}};

int
main()
{
	if(x[0].a != 1) 
		return 1;
	if(x[0].b != 2) 
		return 2;
	return 0;
}
