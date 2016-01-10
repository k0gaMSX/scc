/*
name: TEST043
description: Test for double typedef (taken from plan9 kernel)
output:
F4	0
S2	Clock0link
M6	P	clock
M8	P	link
G9	S2	cl0
F10	I
G11	F10	main
{
\
	G9	M6	.P	@F4	c0
	r	#I0
}
*/

typedef struct Clock0link Clock0link;
typedef struct Clock0link {
	void		(*clock)(void);
	Clock0link*	link;
} Clock0link;



Clock0link cl0;

int
main(void)
{
	(*cl0.clock)();
	return 0;
}
