/* See LICENSE file for copyright and license details. */

/*
name: TEST039
description: Test of integer constants
comments: This test is done for z80 sizes
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"i
A8	N	"u
A9	W	"l
A10	Z	"ul
A11	Q	"ll
A12	O	"ull
	A7	#I1	:I
	A7	#I1	:I
	A8	#N1	:N
	A8	#N1	:N
	A9	#W1	:W
	A9	#W0	:W
	A8	#N0	:N
	A10	#Z1	:Z
	A9	#W1	:W
	A11	#Q0	:Q
	A10	#Z0	:Z
	A12	#O1	:O
	A12	#O1	:O
	h	#I0
}
*/

int
main(void)
{
	int i;
	unsigned u;
	long l;
	unsigned long ul;
	long long ll;
	unsigned long long ull;

	i = 1;
	i = 1u;
	u = 1u;
	u = 1;
	l = 1l;
	l = 0xFFFF + 1;
	u = 0xFFFF + 1;
	ul = 1ul;
	l = 1ul;
	ll = 0xFFFFFFFF + 1;
	ul = 0xFFFFFFFF + 1;
	ull = 1lul;
	ull = 1;
	return 0;
}
