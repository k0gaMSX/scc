/* See LICENSE file for copyright and license details. */

/*
name: TEST030
description: Basic test for vararg functions
error:
output:
V2	K	#NC
V4	K	#N9
S6	"foo	#N18	#N2
M7	I	"i	#N0
M8	I	"j	#N2
M9	I	"k	#N4
M11	P	"p	#N6
M12	J	"v	#N8
G18	I	F	"f1
{
A13	S6	"f
A15	P	"p
A16	I	"n
\
	y	L19	A13	M7	.I	A15	@S6	M7	.I	=I
	h	#I0
L19
	h	A15	@S6	M8	.I	A16	+I
}
G21	I	F	"main
{
\
A22	S6	"f
	A22	M7	.I	A22	M8	.I	#I1	:I	:I
	G18	A22	pS6	A22	'P	pP	#I2	pI	cI
	G18	A22	pS6	A22	'P	pP	#I2	pI	#I1	pI	A22	pS6	A22	'P	pP	cI
	h	#I0
}
*/

struct foo {
	int i, j, k;
	char *p;
	float v;
};

int
f1(struct foo f, struct foo *p, int n, ...)
{
	if (f.i != p->i)
		return 0;
	return p->j + n;
}

int
main(void)
{
	struct foo f;

	f.i = f.j = 1;
	f1(f, &f, 2);
	f1(f, &f, 2, 1, f, &f);

	return 0;
}
