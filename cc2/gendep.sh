#!/bin/sh

gentarget()
{

	cat <<EOF
include target/$1/target.mk
\$(OBJ-$1): \$(HDR)
all: cc2-$1
cc2-$1: \$(OBJ-$1) \$(LIBDIR)/libcc.a
	\$(CC) \$(SCC_LDFLAGS) \$(OBJ-$1) -lcc -o \$@

EOF
}

set -e

rm -f makefile
trap "rm -f $$.mk" 0 2 3

re='\([^-]*\)-\([^-]*\)-\([^-]*\)-\([^-]*\)'

echo $@ |
tr ' ' '\n' |
sed "s/$re/\1-\2/" |
sort -u |
(cat Makefile
while read i
do
	if test ${MKQBE-0} -eq 1
	then
		case $i in
		amd64-sysv)
			gentarget qbe_$i
			;;
		esac
	fi
	gentarget $i
done) > $$.mk && mv $$.mk makefile
