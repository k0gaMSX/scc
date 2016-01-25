/*
name: TEST048
description: Basic test for initializer
error:
output:
S2	S
M3	I	"a
M4	I	"b
V5	S2	#N1
G6	V5	"x
(
	#I1
	#I2
)
G8	F	"main
{
\
	j	L9	G6	M3	.I	#I1	=I
	r	#I1
L9
	j	L11	G6	M4	.I	#I2	=I
	r	#I2
L11
	r	#I0
}
*/

struct S {
	int a;
	int b;
};

struct S x[1] = {{1, 2}};

int
main()
{
	if(x[0].a != 1) 
		return 1;
	if(x[0].b != 2) 
		return 2;
	return 0;
}
