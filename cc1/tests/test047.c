/*
name: TEST047
description: Basic test for initializer
error:
output:
S2	S
M3	I	a
M4	I	b
M5	I	c
G6	S2	x
(
	#I1
	#I2
	#I3
)
F7	I	E
G8	F7	main
{
\
	j	L9	G6	M3	.I	#I1	=I
	r	#I1
L9
	j	L10	G6	M4	.I	#I2	=I
	r	#I2
L10
	j	L11	G6	M5	.I	#I3	=I
	r	#I3
L11
	r	#I0
}
*/



struct S {
	int a;
	int b;
	int c;
};

struct S x = {1, 2, 3};

int
main()
{
	if(x.a != 1) 
		return 1;
	if(x.b != 2) 
		return 2;
	if(x.c != 3) 
		return 3;
	return 0;
}
