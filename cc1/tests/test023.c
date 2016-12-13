/* See LICENSE file for copyright and license details. */

/*
name: TEST023
description: Basic test for long constants
comments: This test is done for z80 data types
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	W	"i
A8	Z	"u
	A7	#W1	:W
	A7	#WFFFFFFFF	:W
	A7	#WFFFFFFFF	:W
	A7	#WFFFF	:W
	A7	#WFFFFFFFF	:W
	A7	#W3	:W
	A7	#W1	:W
	A7	#W0	:W
	A8	#Z1	:Z
	A8	#ZFFFFFFFF	:Z
	A8	#ZFFFFFFFF	:Z
	A8	#ZFFFF	:Z
	A8	#ZFFFFFFFF	:Z
	A8	#Z3	:Z
	A8	#Z1	:Z
	A8	#Z0	:Z
	h	#I0
}
*/

int
main(void)
{
	long i;
	unsigned long u;

	i = 1;
	i = -1;
	i = -1l;
	i = -1u;
	i = -1ll;
	i = (1ll << 32) - 1 & 3;
	i = (long) ((1ll << 32) - 1) < 0;
	i = -1u < 0;

	u = 1;
	u = -1;
	u = -1l;
	u = -1u;
	u = -1ll;
	u = (1ll << 32) - 1 & 3;
	u = (long) ((1ll << 32) - 1) < 0;
	u = -1u < 0;
	return 0;
}
