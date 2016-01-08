/*
name: TEST042
description: Test for bug parsing ternary operators
output:
test042.c:19: error: bad type convertion requested
F1	I
G2	F1	main
{
\
F3	0
X4	F3	f
*/

int
main(void)
{
        void f(void);

        return (int) f();
}
