/* See LICENSE file for copyright and license details. */

/*
name: TEST038
description: Basic test for tentative definitions
error:
test038.c:47: error: redeclaration of 'x'
output:
V2	K	#NC
V4	K	#N9
G5	I	"x
G5	I	"x	(
	#I0
)
X7	I	F	"main
G9	P	F	"foo
{
\
	h	X7	'P
}
G7	I	F	"main
{
\
	G5	#I0	:I
	h	G5
}
*/

int x;
int x = 0;
int x;

int main();

void *
foo()
{
	return &main;
}

int
main()
{
	x = 0;
	return x;
}
int x = 1;
