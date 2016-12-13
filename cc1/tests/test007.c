/* See LICENSE file for copyright and license details. */

/*
name: TEST007
description: basic while test
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"x
	A7	#IA	:I
	j	L10
	e
L8
	A7	A7	#I1	-I	:I
L10
	y	L8	A7	#I0	!I
	b
L9
	h	A7
}
*/

int
main()
{
	int x;

	x = 10;
	while (x)
		x = x - 1;
	return x;
}
