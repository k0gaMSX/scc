/*
name: TEST059
description: Test of initalizers for strings
error:
test059.c:31: warning: initializer-string for array of chars is too long
test059.c:33: error: array of inappropriate type initialized from string constant
output:
V1	K	#4
G2	V1	s0
(
	"666F6F00
)
V4	K	#7
G5	V4	s1
(
	"666F6F00000000
)
V6	K	#2
G7	V6	s2
(
	"666F
)
G9	P	p
(
	"666F6F00	'P
)
*/

char s0[] = "foo";
char s1[7] = "foo";
char s2[2] = "foo";
char *p = "foo";
int m[] = "foo";
