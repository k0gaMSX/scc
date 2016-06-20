#!/bin/sh

tabs 40
for i in *.c
do
	(set -e
         rm -f a.out core
	 scc -m qbe $i
	 ./a.out
        ) 2>/dev/null

        if test $? -eq 0
	then
		st=[OK]
	else
		st=[FAIL]
	fi
	echo $i "\t" $st
done
tabs -8
