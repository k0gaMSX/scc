/* See LICENSE file for copyright and license details. */

/*
name: TEST041
description: Test for bug parsing ternary operators
error:
test041.c:53: error: type mismatch in conditional expression
test041.c:53: error: incompatible types when assigning
test041.c:54: error: used struct/union type value where scalar is required
test041.c:54: warning: expression without side effects
test041.c:55: warning: 'i' defined but not used
test041.c:55: warning: 'foo' defined but not used
test041.c:55: warning: 's' defined but not used
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"i
A9	P	"q
A11	P	"s
A12	P	"p
S14	"foo	#N2	#N1
M15	I	"i	#N0
A16	S14	"foo
	A7	A7	#I0	!I	#W0	#W0	?W	gI	:I
	A12	A7	#I0	!I	#P0	#P0	?P	:P
	A12	A7	#I0	!I	#P0	#P0	?P	:P
	A12	A7	#I0	!I	#P0	#P0	?P	:P
	A9	A7	#I0	!I	#P0	A12	?P	:P
	A9	A7	#I0	!I	A12	#P0	?P	:P
	A9	A7	#I0	!I	A9	#P0	?P	:P
	A9	A7	#I0	!I	#P0	A9	?P	:P
*/
   
int
main(void)
{
	int i, *q;
	char *s;
	void *p;
	struct foo {int i;} foo;

	i = i ? 0 : 0l;
	p = i ? (void *) 0 : 0;
	p = i ? 0 : (void *) 0;
	p = i ? 0 : (const void *) 0;
	q = i ? 0 : p;
	q = i ? p : 0;
	q = i ? q : 0;
	q = i ? 0 : q;
	p = i ? 2 : p;
	foo ? 1 : 2;
}

