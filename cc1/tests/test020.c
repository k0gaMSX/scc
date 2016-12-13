/* See LICENSE file for copyright and license details. */

/*
name: TEST020
description: Basic test for integer algebraic identities
error:
test020.c:84: warning: division by 0
test020.c:85: warning: division by 0
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"i
	A7	A7	#I0	!I	:I
	A7	A7	#I0	!I	#I1	,I	:I
	A7	#I1	:I
	A7	A7	#I0	!I	:I
	A7	A7	#I0	!I	#I0	,I	:I
	A7	A7	#I0	!I	:I
	A7	A7	#I0	!I	:I
	A7	#I0	:I
	A7	A7	:I
	A7	#I0	:I
	A7	A7	:I
	A7	#I0	:I
	A7	A7	:I
	A7	A7	:I
	A7	A7	:I
	A7	#I0	A7	-I	:I
	A7	A7	:I
	A7	A7	:I
	A7	A7	:I
	A7	A7	:I
	A7	A7	#I0	,I	:I
	A7	A7	#I0	,I	:I
	A7	A7	:I
	A7	A7	:I
	A7	A7	:I
	A7	#I1	A7	/I	:I
	A7	A7	:I
	A7	A7	:I
	A7	A7	#I1	,I	:I
	A7	A7	#I0	/I	:I
	A7	A7	#I0	%I	:I
}
*/

int
main(void)
{
	int i;

	i = i || 0;
	i = i || 4;
	i = 4 || i;
	i = 0 || i;
	i = i && 0;
	i = i && 4;
	i = 4 && i;
	i = 0 && i;
	i = i << 0;
	i = 0 << i;
	i = i >> 0;
	i = 0 >> i;
	i = i + 0;
	i = 0 + i;
	i = i - 0;
	i = 0 - i;
	i = i | 0;
	i = 0 | i;
	i = i ^ 0;
	i = 0 ^ i;
	i = i * 0;
	i = 0 * i;
	i = i * 1;
	i = 1 * i;
	i = i / 1;
	i = 1 / i;
	i = i & ~0;
	i = ~0 & i;
	i = i % 1;
	i = i / 0;
	i = i % 0;
}
