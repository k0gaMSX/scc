#!/bin/sh


set -e

while test $# -gt 0
do
	case $1 in
	-c)
		cpu=$2
		shift
		;;
	-f)
		family=$2
		shift
		;;
	*)
		echo gen.sh:incorrect parameter:$1 >&2
		exit 1
		;;
	esac
	shift
done

echo cpu=${cpu:=z80} family=${family:=x80}

rm -f  $$.c target/$family/${cpu}tbl.c
trap "rm -f $$.c" 0 2 3

awk '!/^$/ {print $1,NR,$2,$3,$4,$5,$6}' target/$family/$family.dat |
sort -k1 -k2n |
awk -v cpu=`echo $cpu | tr a-z A-Z` -v family=$family -f gentbl.awk  > $$.c &&
mv $$.c target/$family/${cpu}tbl.c
