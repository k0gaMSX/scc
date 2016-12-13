/* See LICENSE file for copyright and license details. */

/*
name: TEST065
description: Test decay mixed with * operators
error:
test065.c:67: error: increment of pointer to an incomplete type
test065.c:67: error: invalid use of undefined type
test065.c:68: warning: 'argv' defined but not used
output:
V2	K	#NC
V4	K	#N9
G11	I	F	"main
{
A5	I	"argc
A9	P	"argv
\
V12	I	#N1
A13	V12	"v
A16	P	"p
A18	P	"f1
A19	P	"f2
	A13	#I0	:I
	A16	A13	'P	:P
	A18	G11	'P	:P
	A19	G11	'P	:P
	y	L22	A5	#I0	!I
	h	#I1
L22
	y	L23	G11	#I0	pI	#P0	pP	cI	#I0	=I
	h	#I2
L23
	y	L24	A18	@F	#I0	pI	#P0	pP	cI	#I0	=I
	h	#I3
L24
	y	L25	A19	@F	#I0	pI	#P0	pP	cI	#I0	=I
	h	#I4
L25
	y	L26	A16	@I	#I0	!I
	h	#I0
L26
*/

int
main(int argc, char *argv[])
{
	int v[1];
	int (*p)[];
	int (*f1)(int ,char *[]);
	int (*f2)(int ,char *[]);

	v[0] = 0;
	p = &v;
	f1 = &main;
	f2 = main;
	if (argc == 0)
		return 1;
	if ((****main)(0, 0))
		return 2;
	if ((****f1)(0, 0))
		return 3;
	if ((****f2)(0, 0))
		return 4;
	if (!(*p)[0])
		return 0;

	return (*++p)[0] || p[1][0];
}
