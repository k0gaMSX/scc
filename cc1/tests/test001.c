/* See LICENSE file for copyright and license details. */

/*
name: TEST001
description: Basic hello world test
error:
output:
V2	K	#NC
V4	K	#N9
X8	I	F	"printf
G10	I	F	"main
{
\
V13	K	#ND
Y12	V13	"	(
	#"hello world
	#K0A
	#K00
)
	X8	Y12	'P	pP	cI
	h	#I0
}
*/

#include <stdio.h>

int
main(void)
{
	printf("hello world\n");
	return 0;
}
