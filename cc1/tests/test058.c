/*
name: TEST058
description: Test of initialization mixing designators and sequence
error:
output:
V1	I	#5
V2	V1	#3
V3	V2	#2
G4	V3	"arr
(
	#I0
	#I0
	#I3
	#I5
	#I0
	#I1
	#I0
	#I0
	#I6
	#I7
	#I0
	#I0
	#I0
	#I0
	#I0
	#I1
	#I2
	#I0
	#I0
	#I0
	#I0
	#I0
	#I0
	#I0
	#I7
	#I0
	#I0
	#I0
	#I0
	#I0
)
F5	I
G6	F5	"main
{
\
	r	G4	'P	#PA	+P	#P8	+P	@I	G4	'P	#P1E	+P	#PA	+P	#P8	+P	@I	!I
}
*/

int arr[][3][5] = {
	{
		{ 0, 0, 3, 5 },
		{ 1, [3] = 6, 7 },
	},
	{
		{ 1, 2 },
		{ [4] = 7, },
	},
};


int
main(void)
{
	return !(arr[0][1][4] == arr[1][1][4]);
}
