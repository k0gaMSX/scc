/* See LICENSE file for copyright and license details. */

/*
name: TEST003
description: Select function to call inside ternary operator
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"foo
{
\
	h	#I2A
}
G7	I	F	"bar
{
\
	h	#I18
}
G8	I	F	"main
{
\
	h	G6	cI
}
*/

int
foo(void)
{
	return 42;
}

int
bar(void)
{
	return 24;
}

int
main(void)
{
	return (1 ? foo : bar)();
}
