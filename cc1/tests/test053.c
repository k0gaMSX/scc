/* See LICENSE file for copyright and license details. */

/*
name: TEST053
description: Basic test for initializer
error:
output:
V2	K	#NC
V4	K	#N9
S6	"S	#N4	#N1
M7	I	"a	#N0
M8	I	"b	#N2
G9	S6	"s	(
	#I1
	#I2
)
G11	I	F	"main
{
\
	y	L12	G9	M7	.I	#I1	=I
	h	#I1
L12
	y	L13	G9	M8	.I	#I2	=I
	h	#I2
L13
	h	#I0
}
*/

struct S {int a; int b;};
struct S s = { .b = 2, .a = 1};

int
main()
{
	if(s.a != 1)
		return 1;
	if(s.b != 2)
		return 2;
	return 0;
}
