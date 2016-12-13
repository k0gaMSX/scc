/* See LICENSE file for copyright and license details. */

/*
name: TEST008
description: Basic do while loop
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"x
	A7	#I0	:I
	e
L8
	A7	A7	#I1	+I	:I
	y	L8	A7	#IA	<I
	b
L9
	e
L10
	A7	A7	#I1	+I	:I
	y	L10	A7	#I14	<I
	b
L11
	h	A7	#I14	-I
}
*/

int
main()
{
	int x;

	x = 0;
	do
	  x = x + 1;
	while (x < 10);

	do {
	  x = x + 1;
	} while (x < 20);

	return x - 20;
}

