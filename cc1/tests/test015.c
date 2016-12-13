/* See LICENSE file for copyright and license details. */

/*
name: TEST015
description: Stress namespace mechanism
error:
test015.c:56: error: label 's' already defined
output:
V2	K	#NC
V4	K	#N9
S12	"s2	#N2	#N1
M13	I	"s	#N0
S9	"s1	#N4	#N1
M10	I	"s	#N0
M14	S12	"s1	#N2
S6	"s	#N4	#N1
M15	S9	"s	#N0
G16	S6	"s2
G18	I	F	"main
{
\
	j	L19
A20	S6	"s
A21	I	"s
	h	A21
	h	A20	M15	.S9	M10	.I	A20	M15	.S9	M14	.S12	M13	.I	+I
L19
*/

typedef struct s s;

struct s {
	struct s1 {
		int s;
		struct s2 {
			int s;
		} s1;
	} s;
} s2;

#define s s

int
main(void)
{
#undef s
	goto s;
	struct s s;
		{
			int s;
			return s;
		}
	return s.s.s + s.s.s1.s;
	s:
		{
			s: return 0;
		}
}
