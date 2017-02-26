#!/bin/sh

trap 'tabs -8;rm -f a.out' 0 1 2 3 15
tabs 40
ulimit -c 0

while read i state
do
	printf "%s\t" $i
	if test "$state" = TODO
	then
		echo '[TODO]'
	else
		rm -f a.out
		(scc -Isysinclude -m qbe "$i" && ./a.out) 2>/dev/null &&
			echo [OK] || echo [FAILED]
	fi
done
