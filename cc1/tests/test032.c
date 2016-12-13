/* See LICENSE file for copyright and license details. */

/*
name: TEST032
description: test special characters @ and $ in macro definitions
error:
output:
V2	K	#NC
V4	K	#N9
V7	K	#N1E
G9	I	F	"main
{
\
V13	K	#N2C
Y12	V13	"	(
	#"This is a string $ or # or ##and it is ok !
	#K00
)
A11	P	"p
	A11	Y12	'P	:P
	h	A11	#P0	!I
}
*/

#define M1(x) "This is a string $ or # or ##" ## #x

int
main(void)
{
	char *p = M1(and it is ok!);

	return p != 0;
}

