#!/bin/sh

set -e

rm -f makefile
trap "rm -f $$.mk" 0 2 3

(cat Makefile
for i
do
	cat <<EOF
$i: ../../${i%.o}.c
	\$(CC) \$(TF_CFLAGS) ../../${i%.o}.c -c

EOF
done) > $$.mk && mv $$.mk makefile
