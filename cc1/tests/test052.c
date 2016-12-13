/* See LICENSE file for copyright and license details. */

/*
name: TEST052
description: Basic test for initializer
error:
output:
V2	K	#NC
V4	K	#N9
S6	"S	#N4	#N1
M7	I	"a	#N0
M8	I	"b	#N2
V9	S6	#N2
G10	V9	"arr	(
	#I1
	#I2
	#I3
	#I4
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
	y	L16	G10	'P	#P4	+P	@S6	M7	.I	#I3	=I
	h	#I3
L16
	y	L17	G10	'P	#P4	+P	@S6	M8	.I	#I4	=I
	h	#I4
L17
	h	#I0
}
*/

struct S {int a; int b;};
struct S arr[2] = {[1] = {3, 4}, [0] = {1, 2}};

int
main()
{
	if(arr[0].a != 1)
		return 1;
	if(arr[0].b != 2)
		return 2;
	if(arr[1].a != 3)
		return 3;
	if(arr[1].b != 4)
		return 4;
	return 0;
}
