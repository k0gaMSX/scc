/* See LICENSE file for copyright and license details. */

/*
name: TEST002
description: Test forward references before definition of types
error:
output:
V2	K	#NC
V4	K	#N9
G8	P	"x
S6	"S	#N8	#N2
M9	I	"i	#N0
M10	P	"next	#N2
G12	I	F	"main
{
\
A13	S6	"y
A14	P	"p
A15	N	"n
	A13	M9	.I	#I0	:I
	G8	@S6	A13	:S6
	A15	#N0	:N	A14	A13	'P	:P	,P
	j	L18
	e
L16
	A15	#N1	:+N	A14	A14	@S6	M10	.P	:P	,P
L18
	y	L16	A14	#P0	!I
	b
L17
}
*/

struct S *x;
struct S {
	int i;
	struct S *next;
};

int
main(void)
{
	struct S y, *p;
	unsigned n;

	y.i = 0;
	*x = y;

	for (n = 0, p = &y; p; ++n, p = p->next)
		/* nothing */;
}
