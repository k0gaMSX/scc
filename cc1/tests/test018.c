/* See LICENSE file for copyright and license details. */

/*
name: TEST018
description: Basic test for arrays
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
V7	K	#N4
V8	V7	#N2
A9	V8	"arr
A11	P	"p
A13	P	"q
V14	I	#N4
A15	V14	"v
	A11	A9	'P	:P
	A13	A9	'P	#P4	+P	#P3	+P	:P
	A9	'P	#P4	+P	#P3	+P	@K	#K2	:K
	A15	#I2	:I
	y	L17	A9	'P	#P4	+P	#P3	+P	@K	gI	#I2	=I
	h	#I1
L17
	y	L18	A11	#P4	+P	#P3	+P	@K	gI	#I2	=I
	h	#I1
L18
	y	L19	A13	@K	gI	#I2	=I
	h	#I1
L19
	y	L20	A15	#I2	=I
	h	#I1
L20
	h	#I0
}
*/

#line 1

int
main()
{
	char arr[2][4], (*p)[4], *q;
	int v[4];

	p = arr;
	q = &arr[1][3];
	arr[1][3] = 2;
	v[0] = 2;

	if (arr[1][3] != 2)
		return 1;
	if (p[1][3] != 2)
		return 1;
	if (*q != 2)
		return 1;
	if (*v != 2)
		return 1;

	return 0;
}
