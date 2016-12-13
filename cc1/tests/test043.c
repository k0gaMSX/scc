/* See LICENSE file for copyright and license details. */

/*
name: TEST043
description: Test for double typedef (taken from plan9 kernel)
error:
output:
V2	K	#NC
V4	K	#N9
S6	"Clock0link	#N8	#N2
M10	P	"clock	#N0
M12	P	"link	#N2
G13	S6	"cl0
G15	I	F	"main
{
\
	G13	M10	.P	@F	c0
	h	#I0
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
