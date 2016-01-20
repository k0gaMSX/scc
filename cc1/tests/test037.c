
/*
name: TEST037
description: Basic sizeof test
comments: This test is based in z80 sizes
error:
test037.c:30: warning: conditional expression is constant
test037.c:32: warning: conditional expression is constant
test037.c:34: warning: conditional expression is constant
output:
F1	I	E
G2	F1	main
{
\
	j	L3	#I0
	r	#I1
L3
	j	L4	#I0
	r	#I2
L4
	j	L5	#I0
	r	#I3
L5
	r	#I0
}
*/

int main()
{
	if(sizeof(0) != 2)
		return 1;
	if(sizeof(char) != 1)
		return 2;
	if(sizeof(int) != 2)
		return 3;
	return 0;
}
