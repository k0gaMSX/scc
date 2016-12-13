/* See LICENSE file for copyright and license details. */

/*
name: TEST019
description: Basic test of constant folding in integer arithmetic operations
error:
test019.c:12: warning: division by 0
test019.c:13: warning: division by 0
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"i
	A7	#I3	:I
	A7	#I1	:I
	A7	#I12	:I
	A7	#I2	:I
	A7	#I0	:I
	A7	A7	#I0	%I	:I
	A7	A7	#I0	%I	:I
	A7	#I8	:I
	A7	#I2	:I
	A7	#I4	:I
	A7	#IC	:I
	A7	#I8	:I
	A7	#IFFFD	:I
	A7	#IFFF3	:I
	A7	#I1	:I
	A7	#I0	:I
	A7	#I0	:I
	A7	#I1	:I
	A7	#I0	:I
}
*/

#line 1

int
main(void)
{
	int i;

	i = 1 + 2;
	i = 2 - 1;
	i = 3 * 6;
	i = 10 / 5;
	i = 10 % 5;
	i = i % 0;
	i = i % 0;
	i = 1 << 3;
	i = 8 >> 2;
	i = 12 & 4;
	i = 8 | 4;
	i = 12 ^ 4;
	i = -(3);
	i = ~12;
	i = 1 < 3;
	i = 2 > 3;
	i = 2 >= 3;
	i = 2 <= 3;
	i = 1 == 0;
}
