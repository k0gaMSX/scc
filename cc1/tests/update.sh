#!/bin/sh

case $# in
0)
	echo "usage: update.sh test ..." >&2
	exit 1
	;;
*)
	for i
	do
		(echo '/^output/+;/^\*\//-c'
		../cc1 -I./ -w $1 2>&1
		printf ".\nw\n") | ed -s $1
	done
	;;
esac
