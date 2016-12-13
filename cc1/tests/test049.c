/* See LICENSE file for copyright and license details. */

/*
name: TEST049
description: Basic test for initializer
error:
output:
V2	K	#NC
V4	K	#N9
G5	I	"x	(
	#I5
)
G7	P	"p	(
	G5	'P
)
G9	I	F	"main
{
\
	y	L10	G7	@I	#I5	=I
	h	#I1
L10
	h	#I0
}
*/

int x = 5;
int *p = &x;

int
main()
{
	if(*p != 5)
		return 1;
	return 0;
}
