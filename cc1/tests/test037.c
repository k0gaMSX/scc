/* See LICENSE file for copyright and license details. */

/*
name: TEST037
description: Basic sizeof test
comments: This test is based in z80 sizes
error:
test037.c:32: warning: conditional expression is constant
test037.c:34: warning: conditional expression is constant
test037.c:36: warning: conditional expression is constant
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
	y	L7	#I1
	h	#I1
L7
	y	L8	#I1
	h	#I2
L8
	y	L9	#I1
	h	#I3
L9
	h	#I0
}
*/

int main()
{
	if(sizeof(0) != 2)
		return 1;
	if(sizeof(char) != 1)
		return 2;
	if(sizeof(int) != 2)
		return 3;
	return 0;
}
