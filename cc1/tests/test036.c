/* See LICENSE file for copyright and license details. */

/*
name: TEST036
description: Duff's device
error:
test036.c:61: warning: type defaults to 'int' in declaration
output:
V2	K	#NC
V4	K	#N9
G9	I	F	"send
{
R5	P	"to
R6	P	"from
R7	I	"count
\
R11	I	"n
	R11	R7	#I7	+I	#I8	/I	:I
	s	R7	#I8	%I
	v	L13	#I0
L13
	e
L14
	R5	@I	R6	#P2	:iP	@I	:I
	v	L16	#I7
L16
	R5	@I	R6	#P2	:iP	@I	:I
	v	L17	#I6
L17
	R5	@I	R6	#P2	:iP	@I	:I
	v	L18	#I5
L18
	R5	@I	R6	#P2	:iP	@I	:I
	v	L19	#I4
L19
	R5	@I	R6	#P2	:iP	@I	:I
	v	L20	#I3
L20
	R5	@I	R6	#P2	:iP	@I	:I
	v	L21	#I2
L21
	R5	@I	R6	#P2	:iP	@I	:I
	v	L22	#I1
L22
	R5	@I	R6	#P2	:iP	@I	:I
	y	L14	R11	#I1	:-I	#I0	>I
	b
L15
	t	L12
L12
}
*/

/* Disgusting, no?  But it compiles and runs just fine.  I feel a combination of
   pride and revulsion at this discovery.  If no one's thought of it before,
   I think I'll name it after myself.
   It amazes me that after 10 years of writing C there are still
   little corners that I haven't explored fully.
   - Tom Duff */

send(to, from, count)
	register short *to, *from;
	register count;
{
	register n=(count+7)/8;
	switch(count%8){
	case 0:      do{*to = *from++;
	case 7:           *to = *from++;
	case 6:           *to = *from++;
	case 5:           *to = *from++;
	case 4:           *to = *from++;
	case 3:           *to = *from++;
	case 2:           *to = *from++;
	case 1:           *to = *from++;
	                    }while(--n>0);
	}
}
