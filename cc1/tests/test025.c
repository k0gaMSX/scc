/* See LICENSE file for copyright and license details. */

/*
name: TEST025
descritpion: Test of ifdef and ifndef
error:
output:
V2	K	#NC
V4	K	#N9
G5	I	"a
G6	I	"b
G7	I	"c
G8	I	"d
G9	I	"_1
G10	I	"_2
G11	I	"e_
G12	I	"f_
G13	I	"h
G14	I	"i
*/

#define FOO

#ifdef FOO
	int a;
	int b;
	#undef FOO
	#ifndef FOO
		int c;
		int d;
	#else
		int e;
		int f;
	#endif
	int _1;
	int _2;
	#ifdef FOO
		int c_;
		int d_;
	#else
		int e_;
		int f_;
	#endif
	int h;
	int i;
#else
	int j;
	int k;
	#ifdef FOO
		int l;
		int m;
	#else
		int n;
		int o;
	#endif
	int p;
	int q;
	#ifndef FOO
		int r;
		int s;
	#else
		int t;
		int u;
	#endif
	int v;
	int w;
#endif

