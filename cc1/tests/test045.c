/* See LICENSE file for copyright and license details. */

/*
name: TEST045
description: Basic test of initializers
error:
output:
V2	K	#NC
V4	K	#N9
G5	I	"x	(
	#I5
)
G7	I	F	"main
{
\
	y	L8	G5	#I5	=I
	h	#I1
L8
	h	#I0
}
*/



int x = 5;

int
main()
{
	if(x != 5) 
		return 1;
	return 0;
}
