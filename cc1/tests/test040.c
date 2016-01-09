/*
name: TEST040
description: Test for bug parsing typenames in struct definition
output:
test040.c:24: error: unexpected 'List'
F8	I
G9	F8	main
{
\
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

