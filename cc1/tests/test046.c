/* See LICENSE file for copyright and license details. */

/*
name: TEST046
description: Basic test for initializators
error:
output:
V2	K	#NC
V4	K	#N9
V5	I	#N3
G6	V5	"x	(
	#I1
	#I2
	#I3
)
G8	I	F	"main
{
\
	y	L9	G6	#I1	=I
	h	#I1
L9
	y	L11	G6	'P	#P2	+P	@I	#I2	=I
	h	#I2
L11
	y	L12	G6	'P	#P4	+P	@I	#I3	=I
	h	#I3
L12
	h	#I0
}
*/

int x[3] = {1, 2, 3};

int
main()
{
	if(x[0] != 1) 
		return 1;
	if(x[1] != 2) 
		return 2;
	if(x[2] != 3) 
		return 3;
	return 0;
}
