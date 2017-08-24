#!/bin/sh

set -e

rm -f makefile $$.tmp
trap "rm -f $$.mk $$.tmp" 0 2 3

re='\([^-]*\)-\([^-]*\)-\([^-]*\)-\([^-]*\)'

echo $@ |
tr ' 	' '\n' |
sed "s/$re/\1-\2-\3/" |
sort -u |
(cat Makefile
echo
while read i
do
	cat <<EOF >> $$.tmp
	mkdir -p ../../rootdir/lib/scc/crt/$i
	cp $i/crt.o ../../rootdir/lib/scc/crt/$i/
EOF
	echo all: $i/crt.o
done
echo all:
cat $$.tmp) > $$.mk && mv $$.mk makefile
