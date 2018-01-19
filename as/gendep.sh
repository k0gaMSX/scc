#!/bin/sh

set -e

rm -f makefile
trap "rm -f $$.mk" 0 2 3

re='\([^-]*\)-\([^-]*\)-\([^-]*\)-\([^-]*\)'

echo $@ |
tr ' ' '\n' |
sed "s/$re/\1/" |
sort -u |
(cat Makefile
while read i
do
	cat <<EOF
include target/$i.mk

\$(OBJ-$i): \$(HDR)
all: as-$i
OBJ-$i: \$(HDR)
as-$i: \$(OBJ-$i) \$(LIBDIR)/libscc.a
	\$(CC) \$(SCC_LDFLAGS) \$(OBJ-$i) -lscc -o \$@

EOF
done) > $$.mk && mv $$.mk makefile
