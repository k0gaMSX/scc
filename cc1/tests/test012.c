/* See LICENSE file for copyright and license details. */

/*
name: TEST012
description: Basic switch test
error:
test012.c:38: warning: 'foo' defined but not used
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"x
	A7	#I0	:I
	s	A7
	v	L9	#I0
L9
	t	L8
L8
	s	A7
	v	L11	#I0
L11
	s	A7
	v	L13	#I0
L13
	j	L14
	f	L15
L15
	h	#I1
	t	L12
L12
	t	L10
L10
	h	#I2
L14
	s	A7
	v	L17	#I1
L17
	h	#I3
	t	L16
L16
	s	A7
	A7	#I2	:I
L19
	v	L20	#I1
L20
	h	#I4
	t	L18
L18
	s	A7
	v	L22	#I0
L22
	h	A7
	v	L23	#I1
L23
	h	#I1
	f	L24
L24
	h	#I1
	t	L21
L21
}
*/

#line 1

int
main()
{
	int x;

	x = 0;
	switch(x)
	case 0:
		;
	switch(x)
	case 0:
		switch(x) {
		case 0:
			goto next;
		default:
			return 1;
		}
	return 2;
	next:
	switch(x)
	case 1:
		return 3;
	switch(x) {
		x = 1 + 1;
		foo:
	case 1:
		return 4;
	}
	switch(x) {
	case 0:
		return x;
	case 1:
		return 1;
	default:
		return 1;
	}
}
