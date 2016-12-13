/* See LICENSE file for copyright and license details. */

/*
name: TEST042
description: Test for bug parsing ternary operators
error:
test042.c:22: error: bad type conversion requested
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
X8	0	F	"f
*/

int
main(void)
{
        void f(void);

        return (int) f();
}
