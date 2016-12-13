/* See LICENSE file for copyright and license details. */

/*
name: TEST064
description: Test function alike macro without parenthesis
error:
output:
V2	K	#NC
V4	K	#N9
S5	"	#N2	#N1
M6	I	"f	#N0
G9	I	F	"main
{
\
A10	S5	"s
	A10	M6	.I	#I0	:I
	h	A10	M6	.I
}
*/

#define x f
#define y() f

typedef struct { int f; } S;

int
main()
{
	S s;
	
	s.x = 0;
	return s.y();
}

