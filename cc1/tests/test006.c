/* See LICENSE file for copyright and license details. */

/*
name: TEST006
description: Basic test for if
error:
test006.c:5: warning: conditional expression is constant
test006.c:7: warning: conditional expression is constant
test006.c:10: warning: conditional expression is constant
output:
V2	K	#NC
V4	K	#N9
G5	K	"c
G7	I	F	"main
{
\
	y	L8	#I1
	h	#I1
	j	L9
L8
	y	L10	#I1
	j	L11
L10
	y	L12	#I0
	y	L13	G5	gI	#I0	=I
	h	#I1
	j	L14
L13
	h	#I0
L14
	j	L15
L12
	h	#I1
L15
L11
L9
	h	#I1
}
*/

char c;

#line 1

int
main()
{
	if(0) {
		return 1;
	} else if(0) {
		/* empty */
	} else {
		if(1) {
			if(c)
				return 1;
			else
				return 0;
		} else {
			return 1;
		}
	}
	return 1;
}
