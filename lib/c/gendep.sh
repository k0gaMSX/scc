#!/bin/sh

sed 's/\.o/.c/' ../obj.lst |
while read src
do
	echo '#include "../src/'$src'"' > $src
done

(cat Makefile
echo
printf "OBJ ="
while read i
do
	printf " %s" $i
done < ../obj.lst
echo) > makefile

