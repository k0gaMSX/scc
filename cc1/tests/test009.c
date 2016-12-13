/* See LICENSE file for copyright and license details. */

/*
name: TEST009
description: Basic test for loops
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"x
	A7	#I0	:I
	j	L10
	e
L8
	A7	A7	#I1	+I	:I
L10
	y	L8	A7	#IA	<I
	b
L9
	y	L11	A7	#IA	=I
	h	#I1
L11
	h	#I0
}
*/

int
main()
{
	int x;

	for (x = 0; x < 10 ; x = x + 1)
		;
	if (x != 10)
		return 1;
	return 0;
}

