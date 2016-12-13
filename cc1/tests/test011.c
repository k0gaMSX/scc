/* See LICENSE file for copyright and license details. */

/*
name: TEST011
description: Basic test for goto
error:
test011.c:13: warning: 'foo' defined but not used
test011.c:13: warning: 'start' defined but not used
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
L7
	j	L8
	h	#I1
L9
	h	#I0
L8
L10
	j	L9
	h	#I1
}
*/

#line 1

int
main() {
	start:
		goto next;
		return 1;
	success:
		return 0;
	next:
	foo:
		goto success;
		return 1;
}
