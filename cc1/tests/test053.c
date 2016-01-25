/*
name: TEST053
description: Basic test for initializer
error:
output:
S2	"S	#N4	#N1
M3	I	"a	#N0
M4	I	"b	#N2
G5	S2	"s	(
	#I1
	#I2
)
G7	F	"main
{
\
	j	L8	G5	M3	.I	#I1	=I
	r	#I1
L8
	j	L9	G5	M4	.I	#I2	=I
	r	#I2
L9
	r	#I0
}
*/

struct S {int a; int b;};
struct S s = { .b = 2, .a = 1};

int
main()
{
	if(s.a != 1)
		return 1;
	if(s.b != 2)
		return 2;
	return 0;
}
