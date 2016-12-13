/* See LICENSE file for copyright and license details. */

/*
name: TEST047
description: Basic test for initializer
error:
output:
V2	K	#NC
V4	K	#N9
S6	"S	#N6	#N1
M7	I	"a	#N0
M8	I	"b	#N2
M9	I	"c	#N4
G10	S6	"x	(
	#I1
	#I2
	#I3
)
G12	I	F	"main
{
\
	y	L13	G10	M7	.I	#I1	=I
	h	#I1
L13
	y	L14	G10	M8	.I	#I2	=I
	h	#I2
L14
	y	L15	G10	M9	.I	#I3	=I
	h	#I3
L15
	h	#I0
}
*/



struct S {
	int a;
	int b;
	int c;
};

struct S x = {1, 2, 3};

int
main()
{
	if(x.a != 1) 
		return 1;
	if(x.b != 2) 
		return 2;
	if(x.c != 3) 
		return 3;
	return 0;
}
