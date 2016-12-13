/* See LICENSE file for copyright and license details. */

/*
name: TEST024
description: Basic test for long long constants
comments: This test is done for z80 data types
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	Q	"i
A8	O	"u
	A7	#Q1	:Q
	A7	#QFFFFFFFFFFFFFFFF	:Q
	A7	#QFFFFFFFFFFFFFFFF	:Q
	A7	#QFFFF	:Q
	A7	#QFFFFFFFFFFFFFFFF	:Q
	A7	#Q3	:Q
	A7	#Q1	:Q
	A8	#O1	:O
	A8	#OFFFFFFFFFFFFFFFF	:O
	A8	#OFFFFFFFFFFFFFFFF	:O
	A8	#OFFFF	:O
	A8	#OFFFFFFFFFFFFFFFF	:O
	A8	#O3	:O
	A8	#O0	:O
	h	#I0
}
*/

int
main(void)
{
	long long i;
	unsigned long long u;

	i = 1;
	i = -1;
	i = -1l;
	i = -1u;
	i = -1ll;
	i = -1ll & 3;
	i = -1ll < 0;

	u = 1;
	u = -1;
	u = -1l;
	u = -1u;
	u = -1ll;
	u = -1llu & 3;
	u = -1llu < 0;
	return 0;
}
