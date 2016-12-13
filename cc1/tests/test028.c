/* See LICENSE file for copyright and license details. */

/*
name: TEST028
description: Test of reinterpretation in define
error:
output:
V2	K	#NC
V4	K	#N9
G10	P	F	"foo
{
\
V12	K	#N3
Y14	V12	"	(
	#"hi
	#K00
)
	h	Y14	'P
}
*/


#define M(x) x
#define A(a,b) a(b)

char *
foo(void)
{
	return A(M,"hi");
}
