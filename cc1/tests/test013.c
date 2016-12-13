/* See LICENSE file for copyright and license details. */

/*
name: TEST013
description: Basic test of integer types and integer conversions
comments: This test depends of the configuration in the type system.
          With the current configuration char is equal to unsigned char,
          short is equal to int, and unsigned short is equal to unsigned.
error:
output:
V2	K	#NC
V4	K	#N9
G5	I	"a
G6	N	"b
G7	K	"c
G8	C	"d
G9	K	"e
G10	W	"f
G11	Z	"g
G12	Q	"h
G13	O	"i
G14	I	"j
G15	N	"k
G17	I	F	"main
{
\
	G5	G6	gI	:I
	G5	G7	gI	:I
	G5	G8	gI	:I
	G5	G9	gI	:I
	G5	G10	gI	:I
	G5	G11	gI	:I
	G5	G12	gI	:I
	G5	G13	gI	:I
	G5	G14	:I
	G5	G15	gI	:I
	G6	G5	gN	:N
	G6	G7	gN	:N
	G6	G8	gN	:N
	G6	G9	gN	:N
	G6	G10	gN	:N
	G6	G11	gN	:N
	G6	G12	gN	:N
	G6	G13	gN	:N
	G6	G14	gN	:N
	G6	G15	:N
	G7	G5	gK	:K
	G7	G6	gK	:K
	G7	G8	gK	:K
	G7	G9	:K
	G7	G10	gK	:K
	G7	G11	gK	:K
	G7	G12	gK	:K
	G7	G13	gK	:K
	G7	G14	gK	:K
	G7	G15	gK	:K
	G8	G5	gC	:C
	G8	G6	gC	:C
	G8	G7	gC	:C
	G8	G9	gC	:C
	G8	G10	gC	:C
	G8	G11	gC	:C
	G8	G12	gC	:C
	G8	G13	gC	:C
	G8	G14	gC	:C
	G8	G15	gC	:C
	G9	G5	gK	:K
	G9	G6	gK	:K
	G9	G7	:K
	G9	G8	gK	:K
	G9	G10	gK	:K
	G9	G11	gK	:K
	G9	G12	gK	:K
	G9	G13	gK	:K
	G9	G14	gK	:K
	G9	G15	gK	:K
	G10	G5	gW	:W
	G10	G6	gW	:W
	G10	G7	gW	:W
	G10	G8	gW	:W
	G10	G9	gW	:W
	G10	G11	gW	:W
	G10	G12	gW	:W
	G10	G13	gW	:W
	G10	G14	gW	:W
	G10	G15	gW	:W
	G11	G5	gZ	:Z
	G11	G6	gZ	:Z
	G11	G7	gZ	:Z
	G11	G8	gZ	:Z
	G11	G9	gZ	:Z
	G11	G10	gZ	:Z
	G11	G12	gZ	:Z
	G11	G13	gZ	:Z
	G11	G14	gZ	:Z
	G11	G15	gZ	:Z
	G12	G5	gQ	:Q
	G12	G6	gQ	:Q
	G12	G7	gQ	:Q
	G12	G8	gQ	:Q
	G12	G9	gQ	:Q
	G12	G10	gQ	:Q
	G12	G11	gQ	:Q
	G12	G13	gQ	:Q
	G12	G14	gQ	:Q
	G12	G15	gQ	:Q
	G13	G5	gO	:O
	G13	G6	gO	:O
	G13	G7	gO	:O
	G13	G8	gO	:O
	G13	G9	gO	:O
	G13	G10	gO	:O
	G13	G11	gO	:O
	G13	G12	gO	:O
	G13	G14	gO	:O
	G13	G15	gO	:O
	G14	G5	:I
	G14	G6	gI	:I
	G14	G7	gI	:I
	G14	G8	gI	:I
	G14	G9	gI	:I
	G14	G10	gI	:I
	G14	G11	gI	:I
	G14	G12	gI	:I
	G14	G13	gI	:I
	G14	G15	gI	:I
	G15	G5	gN	:N
	G15	G6	:N
	G15	G7	gN	:N
	G15	G8	gN	:N
	G15	G9	gN	:N
	G15	G10	gN	:N
	G15	G11	gN	:N
	G15	G12	gN	:N
	G15	G14	gN	:N
	G15	G13	gN	:N
}
*/

int a;
unsigned b;
char c;
signed char d;
unsigned char e;
long f;
unsigned long g;
long long h;
unsigned long long i;
short j;
unsigned short k;

int
main(void)
{
	a = b;
	a = c;
	a = d;
	a = e;
	a = f;
	a = g;
	a = h;
	a = i;
	a = j;
	a = k;

	b = a;
	b = c;
	b = d;
	b = e;
	b = f;
	b = g;
	b = h;
	b = i;
	b = j;
	b = k;

	c = a;
	c = b;
	c = d;
	c = e;
	c = f;
	c = g;
	c = h;
	c = i;
	c = j;
	c = k;

	d = a;
	d = b;
	d = c;
	d = e;
	d = f;
	d = g;
	d = h;
	d = i;
	d = j;
	d = k;

	e = a;
	e = b;
	e = c;
	e = d;
	e = f;
	e = g;
	e = h;
	e = i;
	e = j;
	e = k;

	f = a;
	f = b;
	f = c;
	f = d;
	f = e;
	f = g;
	f = h;
	f = i;
	f = j;
	f = k;

	g = a;
	g = b;
	g = c;
	g = d;
	g = e;
	g = f;
	g = h;
	g = i;
	g = j;
	g = k;

	h = a;
	h = b;
	h = c;
	h = d;
	h = e;
	h = f;
	h = g;
	h = i;
	h = j;
	h = k;

	i = a;
	i = b;
	i = c;
	i = d;
	i = e;
	i = f;
	i = g;
	i = h;
	i = j;
	i = k;

	j = a;
	j = b;
	j = c;
	j = d;
	j = e;
	j = f;
	j = g;
	j = h;
	j = i;
	j = k;

	k = a;
	k = b;
	k = c;
	k = d;
	k = e;
	k = f;
	k = g;
	k = h;
	k = j;
	k = i;
}
