/*
name: TEST040
description: Test for bug parsing typenames in struct definition
output:

*/

typedef struct List List;
struct List {
	int len;
	struct List *head;
};

