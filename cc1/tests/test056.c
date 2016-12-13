/* See LICENSE file for copyright and license details. */

/*
name: TEST056
desciption: Test of overlaying designators
error:
output:
V2	K	#NC
V4	K	#N9
V10	K	#N3
S6	"S	#NB	#N1
M7	I	"a	#N0
M8	I	"b	#N2
M9	I	"c	#N4
M11	V10	"d	#N6
M12	I	"e	#N9
G13	S6	"s	(
	#I1
	#I2
	#I0
	#K4
	#K6
	#K0
	#I0
)
G15	V14	"m	(
)
G17	I	F	"main
{
\
	h	G13	M11	.V10	'P	#P2	+P	@K	gI	gN	#N0	=I
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
