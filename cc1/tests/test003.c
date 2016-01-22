/*
name: TEST003
description: Select function to call inside ternary operator
error:
output:
G2	F	"foo
{
\
	r	#I2A
}
G3	F	"bar
{
\
	r	#I18
}
G4	F	"main
{
\
	r	G2	cI
}
*/

int
foo(void)
{
	return 42;
}

int
bar(void)
{
	return 24;
}

int
main(void)
{
	return (1 ? foo : bar)();
}
