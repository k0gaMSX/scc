#!/bin/sh

set -e

rm -f makefile
trap "rm -f $$.mk" 0 2 3

re='\([^-]*\)-\([^-]*\)-\([^-]*\)-\([^-]*\)'

echo $@ |
tr ' 	' '\n' |
sed "s/$re/\1-\2/" |
sort -u |
(cat Makefile
echo
while read i
do
	cat <<EOF
all: cc1-$i
OBJ-$i= \$(OBJ)  target/$i/arch.o
\$(OBJ-$i): \$(HDR)
cc1-$i: \$(OBJ-$i) \$(LIBDIR)/libscc.a
	\$(CC) \$(SCC_LDFLAGS) \$(OBJ-$i) -lscc -o \$@

EOF
	echo
done) > $$.mk && mv $$.mk makefile
