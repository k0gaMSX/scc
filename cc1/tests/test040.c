/* See LICENSE file for copyright and license details. */

/*
name: TEST040
description: Test for bug parsing typenames in struct definition
error:
output:
V2	K	#NC
V4	K	#N9
S6	"List	#NC	#N2
M8	I	"len	#N0
M10	P	"head	#N2
M11	P	"back	#N4
G13	I	F	"main
{
\
A14	S6	"List
	h	A14	M8	.I
}
*/

typedef struct List List;
struct List {
	int len;
	struct List *head;
	List *back;
};

int
main(void)
{
	List List;

	return List.len;
}

