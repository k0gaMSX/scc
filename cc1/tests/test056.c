/*
name: TEST056
desciption: Test of overlaying designators
error:
output:
V6	K	#3
S2	S
M3	I	"a
M4	I	"b
M5	I	"c
M7	V6	"d
M8	I	"e
G9	S2	"s
(
	#I1
	#I2
	#I0
	#K4
	#K6
	#K0
	#I0
)
V10	K	#0
G11	V10	"m
(
)
G13	F	"main
{
\
	r	G9	M7	.V6	'P	#P2	+P	@K	gI	gN	#N0	!I
}
*/

struct S {
	int a, b, c;
	char d[3];
	int e;
} s = {
	.a = 1,
	.b = 2,
	.d = {[0] = 3, [2] = 5},
	.d = {[0] = 4, [1] = 6}
};

char m[] = {};

int
main(void)
{
	return sizeof(m) == s.d[2];
}
