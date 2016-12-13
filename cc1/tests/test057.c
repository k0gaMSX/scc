/* See LICENSE file for copyright and license details. */

/*
name: TEST057
description: test of initialization of incomplete arrays
error:
output:
V2	K	#NC
V4	K	#N9
V5	I	#N3
G7	V6	"arr1	(
	#I2
	#I7
	#I5
	#I5
	#I1
	#I2
)
G8	V6	"arr2	(
	#I2
	#I7
	#I5
	#I5
	#I1
	#I2
)
G10	I	F	"main
{
\
	h	G7	'P	#P6	+P	#P4	+P	@I	G8	'P	#P6	+P	#P6	+P	@I	!I
}
*/

int arr1[][3] = {
                 { 2, 7, 5, },
                 { 5, 1, 2, },
};

int arr2[2][3] = {
	2, 7, 5,
	5, 1, 2
};

int
main(void)
{
	return !(arr1[1][2] == arr2[1][3]);
}
