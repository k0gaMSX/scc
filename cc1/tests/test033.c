/* See LICENSE file for copyright and license details. */

/*
name: TEST033
description: test for #if defined()
error:
output:
V2	K	#NC
V4	K	#N9
G5	I	"c
*/

#if defined(FOO)
int a;
#elif !defined(FOO) && defined(BAR)
int b;
#elif !defined(FOO) && !defined(BAR)
int c;
#else
int d;
#endif

