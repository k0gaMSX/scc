/* See LICENSE file for copyright and license details. */

/*
name: TEST065
description: Test decay mixed with * operators
error:
test065.c:65: error: increment of pointer to an incomplete type
test065.c:65: error: invalid use of undefined type
test065.c:66: warning: 'argv' defined but not used
output:
G7	I	F	"main
{
A1	I	"argc
A5	P	"argv
\
V8	I	#N1
A9	V8	"v
A12	P	"p
A18	P	"f1
A21	P	"f2
	A9	#I0	:I
	A12	A9	'P	:P
	A18	G7	'P	:P
	A21	G7	'P	:P
	y	L26	A1	#I0	!I
	h	#I1
L26
	y	L27	G7	#I0	pI	#P0	pP	cI	#I0	=I
	h	#I2
L27
	y	L28	A18	@F	#I0	pI	#P0	pP	cI	#I0	=I
	h	#I3
L28
	y	L29	A21	@F	#I0	pI	#P0	pP	cI	#I0	=I
	h	#I4
L29
	y	L30	A12	@I	#I0	!I
	h	#I0
L30
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
