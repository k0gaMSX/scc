
/*
name: TEST038
description: Basic test for tentative definitions
error:
test038.c:46: error: redeclaration of 'x'
output:
G1	I	x
(
	#I0
)
F2	I	E
X3	F2	main
F4	P	E
G5	F4	foo
{
\
	r	X3	'P
}
G3	F2	main
{
\
	G1	#I0	:I
	r	G1
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
