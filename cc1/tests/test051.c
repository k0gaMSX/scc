/* See LICENSE file for copyright and license details. */

/*
name: TEST051
description: Basic test for initializer
error:
output:
V2	K	#NC
V4	K	#N9
V5	I	#N3
G6	V5	"arr	(
	#I0
	#I1
	#I2
)
G8	I	F	"main
{
\
	y	L9	G6	#I0	=I
	h	#I1
L9
	y	L11	G6	'P	#P2	+P	@I	#I1	=I
	h	#I2
L11
	y	L12	G6	'P	#P4	+P	@I	#I2	=I
	h	#I3
L12
	h	#I0
}
*/

int arr[3] = {[2] = 2, [0] = 0, [1] = 1};

int
main()
{
	if(arr[0] != 0)
		return 1;
	if(arr[1] != 1)
		return 2;
	if(arr[2] != 2)
		return 3;
	return 0;
}
