/* See LICENSE file for copyright and license details. */

/*
name: TEST017
description: Basic test about pointers and structs
error:
output:
V2	K	#NC
V4	K	#N9
S6	"s1	#N4	#N1
M7	I	"y	#N0
M8	I	"z	#N2
S10	"s2	#N4	#N2
M12	P	"p	#N0
G14	I	F	"main
{
\
A15	S6	"nested
A16	S10	"v
	A16	M12	.P	A15	'P	:P
	A16	M12	.P	@S6	M7	.I	#I1	:I
	A16	M12	.P	@S6	M8	.I	#I2	:I
	y	L17	A15	M7	.I	#I1	=I
	h	#I1
L17
	y	L18	A15	M8	.I	#I2	=I
	h	#I2
L18
	h	#I0
}
*/

#line 1

struct s1 {
    int y;
    int z;
};

struct s2 {
    struct s1 *p;
};

int main()
{
    struct s1 nested;
    struct s2 v;
    v.p = &nested;
    v.p->y = 1;
    v.p->z = 2;
    if (nested.y != 1)
        return 1;
    if (nested.z != 2)
        return 2;
    return 0;
}
