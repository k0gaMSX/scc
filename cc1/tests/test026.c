/* See LICENSE file for copyright and license details. */

/*
name: TEST026
descritpion: Test of predefined cpp macros
error:
output:
V2	K	#NC
V4	K	#N9
G7	I	F	"main
{
\
A8	I	"y
A10	P	"p
V12	K	#NA
Y11	V12	"	(
	#"test026.c
	#K00
)
	A10	Y11	'P	:P
	A8	#I26	:I
	A8	#I1	:I
	A8	#I1	:I
	A8	#I1	:I
	A8	#I1	:I
}
*/

#define x(y)  (y)

int
main(void)
{
	int y;
	char *p;

	p = __FILE__;
	y = __LINE__;
/*	p = __DATE__;   __DATE__ generates  different value each time */
	y = __STDC__;
/*	p = __TIME__;   __TIME__ generates  different value each time */
	y = __STDC_HOSTED__;
	y = __SCC__;
	y = x(1);
}
