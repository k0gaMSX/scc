/* See LICENSE file for copyright and license details. */

/*
name: TEST035
description: Basic test for enumerations
error:
output:
V2	K	#NC
V4	K	#N9
*/

enum E {
	x,
	y = 2,
	z,
};


int
main()
{
	enum E e = 3;

	if (e !=1)
		return 0;
	if (e != x)
		return 1;
	if (e != y)
		return 2;
	if (e != z)
		return 3;
	
	return x;
}
