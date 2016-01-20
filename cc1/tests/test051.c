/*
name: TEST051
description: Basic test for initializer
error:
output:
V1	I	#3
G2	V1	arr
(
	#I0
	#I1
	#I2
)
F3	I	E
G4	F3	main
{
\
	j	L5	G2	#I0	=I
	r	#I1
L5
	j	L7	G2	'P	#P2	+P	@I	#I1	=I
	r	#I2
L7
	j	L8	G2	'P	#P4	+P	@I	#I2	=I
	r	#I3
L8
	r	#I0
}
*/

int arr[3] = {[2] = 2, [0] = 0, [1] = 1};

int
main()
{
	if(arr[0] != 0)
		return 1;
	if(arr[1] != 1)
		return 2;
	if(arr[2] != 2)
		return 3;
	return 0;
}
