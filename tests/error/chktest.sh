#!/bin/sh

err=/tmp/$$.err
chk=/tmp/$$.chk

trap "tabs -8;rm -f a.out *.o $err" 0 1 2 3 15
tabs 40
ulimit -c 0
rm -f test.log

while read i state
do
	echo $i >> test.log
	printf "%s\t" $i
	printf "%s" $state

	scc $CFLAGS -w -c "$i" 2> $err
	echo '/^PATTERN/+;/^\./-p' | ed -s $i > $chk
	diff -c $chk $err >> test.log  && echo [OK] || echo [FAILED]
	rm -f *.o
done
