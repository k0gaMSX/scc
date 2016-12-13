/* See LICENSE file for copyright and license details. */

/*
name: TEST027
description: Test of cpp stringizer
error:
output:
V2	K	#NC
V4	K	#N9
G7	I	F	"main
{
\
A9	P	"p
V11	K	#N19
Y10	V11	"	(
	#"hello is better than bye
	#K00
)
	A9	Y10	'P	:P
	h	A9	@K	gI
}
*/

#define x(y) #y

int
main(void)
{
	char *p;
	p = x(hello)  " is better than bye";

	return *p;
}
