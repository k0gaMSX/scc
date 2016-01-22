
/*
name: TEST035
description: Basic test for enumerations
error:
output:
F6	I	E
G7	F6	"main
{
\
A8	I	"e
	A8	#I3	:I
	j	L9	A8	#I1	=I
	r	#I0
L9
	j	L10	A8	#I0	=I
	r	#I1
L10
	j	L11	A8	#I2	=I
	r	#I2
L11
	j	L12	A8	#I3	=I
	r	#I3
L12
	r	#I0
}
*/

enum E {
	x,
	y = 2,
	z,
};


int
main()
{
	enum E e = 3;

	if (e !=1)
		return 0;
	if (e != x)
		return 1;
	if (e != y)
		return 2;
	if (e != z)
		return 3;
	
	return x;
}
