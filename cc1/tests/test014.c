/* See LICENSE file for copyright and license details. */

/*
name: TEST014
description: Basic storage class test
error:
test014.c:15: warning: 'a' defined but not used
test014.c:15: warning: 'k' defined but not used
test014.c:15: warning: 'j' defined but not used
test014.c:15: warning: 'i' defined but not used
test014.c:15: warning: 'h' defined but not used
test014.c:21: warning: 'par' defined but not used
test014.c:21: warning: 'par' defined but not used
test014.c:26: warning: 'par' defined but not used
test014.c:28: error: incorrect storage class for file-scope declaration
test014.c:31: error: bad storage class in function parameter
test014.c:32: error: invalid storage class for function 'func4'
test014.c:33: error: invalid type specification
test014.c:34: warning: 'f' defined but not used
test014.c:34: warning: 'par' defined but not used
test014.c:37: error: conflicting types for 'd'
output:
V2	K	#NC
V4	K	#N9
G5	I	"a
Y6	K	"b
X7	I	"c
G10	I	F	"func1
{
\
A11	I	"h
T12	K	"i
R13	W	"j
X14	I	"k
T15	Z	"a
	h	#I0
}
G18	0	F	"func2
{
R16	I	"par
\
A19	I	"par
}
T21	0	F	"func3
{
R20	I	"par
\
}
*/

#line 1
int a;
static char b;
extern int c;
typedef unsigned e;

int
func1(void)
{
	auto h;
	static char i;
	register long j;
	extern int k;
	static unsigned long a;
	return 0;
}

void
func2(register int par)
{
	int par;
}

static void
func3(register int par)
{
}

register short d;

register void
func4(static int par)
{
	static register f;
}

short d;
char d;

