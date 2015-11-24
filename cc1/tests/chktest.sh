#!/bin/sh

out=/tmp/$$.out
chk=/tmp/$$.chk

trap "rm -f $out $chk" EXIT INT QUIT HUP
rm -f test.log

for i in *.c
do
	rm -f $out $chk
	awk '/^name:/    {printf "Running %s ", $2}
	     /^output:$/ {copyon=1; next}
	     /^\*\//     {copyon=0; next}
	     copyon==1   {print > "'$chk'"}' $i

	../cc1 -I. -w $i > $out 2>&1
	if diff -c $chk $out >> test.log
	then
		echo [OK]
	else
		echo [FAILED]
	fi
done
