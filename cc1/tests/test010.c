/* See LICENSE file for copyright and license details. */

/*
name: TEST010
description: Test for continue and break statements
error:
test010.c:8: warning: conditional expression is constant
test010.c:10: warning: conditional expression is constant
test010.c:30: warning: conditional expression is constant
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"x
	A7	#I0	:I
	j	L10
	e
L8
	j	L9
L10
	y	L8	#I1
	b
L9
	j	L13
	e
L11
	y	L14	A7	#I5	!I
	j	L12
L14
	A7	A7	#I1	+I	:I
	j	L11
L13
	y	L11	#I1
	b
L12

	j	L17
	e
L15
	y	L18	A7	#IA	!I
	j	L16
L18
	A7	A7	#I1	+I	:I
	j	L15

L17
	j	L15

	b
L16
	e
L19
	y	L21	A7	#IF	!I
	j	L20
L21
	A7	A7	#I1	+I	:I
	j	L19
	y	L19	#I1
	b
L20
	h	A7	#IF	-I
}
*/

#line 1

int
main()
{
	int x;

	x = 0;
	while(1)
		break;
	while(1) {
		if (x == 5) {
			break;
		}
		x = x + 1;
		continue;
	}
	for (;;) {
		if (x == 10) {
			break;
		}
		x = x + 1;
		continue;
	}
	do {
		if (x == 15) {
			break;
		}
		x = x + 1;
		continue;
	} while(1);
	return x - 15;
}
