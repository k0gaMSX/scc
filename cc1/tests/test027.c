
/*
name: TEST027
description: Test of cpp stringizer
error:
output:
F2	I
G3	F2	"main
{
\
A5	P	"p
V7	K	#25
Y6	V7	"
(
	#"hello is better than bye
	#K00
)
	A5	Y6	'P	:P
	r	A5	@K	gI
}
*/

#define x(y) #y

int
main(void)
{
	char *p;
	p = x(hello)  " is better than bye";

	return *p;
}
