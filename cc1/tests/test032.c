
/*
name: TEST032
description: test special characters @ and $ in macro definitions
error:
output:
F4	I
G5	F4	main
{
\
V9	K	#44
Y8	V9
(
	#"This is a string $ or # or ##and it is ok !
	#K00
)
A7	P	p
	A7	Y8	'P	:P
	r	A7	#P0	!I
}
*/

#define M1(x) "This is a string $ or # or ##" ## #x

int
main(void)
{
	char *p = M1(and it is ok!);

	return p != 0;
}

