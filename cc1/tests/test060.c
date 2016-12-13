/* See LICENSE file for copyright and license details. */

/*
name: TEST060
description: Test for correctness of #line
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
	h	#I0
}
*/

#undef  line
#define line 1000

#line line
#if 1000 != __LINE__
	#error "  # line line" not work as expected
#endif

int
main()
{
	return 0;
}

