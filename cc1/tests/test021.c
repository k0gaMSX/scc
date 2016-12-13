/* See LICENSE file for copyright and license details. */

/*
name: TEST021
description: Basic test for char constants
comments: This test is done for z80 implementation
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	K	"uc
A8	C	"sc
	A7	#KFF	:K
	A7	#K23	:K
	A7	#K1	:K
	A7	#K1	:K
	A7	#K41	:K
	A8	#CFF	:C
	A8	#C23	:C
	A8	#C1	:C
	A8	#C1	:C
	A8	#C41	:C
}
*/

int
main(void)
{
	unsigned char uc;
	signed char sc;

	uc = -1;
	uc = '\x23';
	uc = 1u;
	uc = 1025;
	uc = 'A';

	sc = -1;
	sc = '\x23';
	sc = 1u;
	sc = 1025;
	sc = 'A';
}
