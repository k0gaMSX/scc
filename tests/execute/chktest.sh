#!/bin/sh

trap 'tabs -8;rm -f a.out' 0 1 2 3 15
tabs 40
ulimit -c 0

while read i state
do
	printf "%s\t" $i
	printf "%s" $state
	rm -f a.out
	(scc -Isysinclude $CFLAGS "$i" && ./a.out) 2>/dev/null &&
		echo [OK] || echo [FAILED]
done
