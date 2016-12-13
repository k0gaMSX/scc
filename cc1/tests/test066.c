/* See LICENSE file for copyright and license details. */

/*
name: TEST066
description: Test cpp defined operator
error:
test066.c:55: error: operator 'defined' requires an identifier
test066.c:55: error: expected ')' before '<EOF>'
output:
V2	K	#NC
V4	K	#N9
G5	I	"x	(
	#I0
)
G7	I	F	"main
{
\
	h	#I0
*/


#if defined X
X
#endif

#if defined(X)
X
#endif

#if X
X
#endif

#define X 0

#if X
X
#endif

#if defined(X)
int x = 0;
#endif

#undef X
#define X 1

#if X
int
main()
{
	return 0;
}
#endif

#if defined (1)
#error 1 is defined
#endif
