/* See LICENSE file for copyright and license details. */

/*
name: TEST005
description: Test unary integer operations
error:
output:
V2	K	#NC
V4	K	#N9
G6	I	F	"main
{
\
A7	I	"x
	A7	#I3	:I
	A7	A7	#I0	=I	:I
	A7	A7	#I0	=I	:I
	A7	A7	~I	:I
	A7	A7	_I	:I
	y	L8	A7	#I2	=I
	h	#I1
L8
	h	#I0
}
*/


int
main()
{
	int x;

	x = 3;
	x = !x; //  0
	x = !x; //  1
	x = ~x; // -1
	x = -x; //  2
	if(x != 2)
		return 1;
	return 0;
}
