/*
name: TEST001
description: Basic hello world test
error:
output:
F3	I	P	E
X4	F3	printf
F5	I
G6	F5	main
{
\
V8	K	#13
Y7	V8
(
	#"hello world
	#K0A
	#K00
)
	X4	Y7	'P	pP	cI
	r	#I0
}
*/

#include <stdio.h>

int
main(void)
{
	printf("hello world\n");
	return 0;
}
