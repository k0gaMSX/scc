/*
name: TEST040
description: Test for bug parsing typenames in struct definition
error:
output:
F8	I
G9	F8	main
{
\
S2	List
M4	I	len
M6	P	head
M7	P	back
A10	S2	List
	r	A10	M4	.I
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

