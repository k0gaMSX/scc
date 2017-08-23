#!/bin/sh

set -e

rm -f config.h
trap "rm -f $$.h" 0 2 3

PREFIX=${PREFIX-$HOME}
USEQBE=${USEQBE:-1}

echo $@ |
(IFS='- 	' read arch abi sys format r
echo \#define PREFIX \"$PREFIX\"
echo \#define USEQBE $USEQBE
echo \#define ARCH \"$arch\"
echo \#define SYS  \"$sys\"
echo \#define ABI  \"$abi\"
echo \#define FORMAT \"$format\") > $$.h && mv $$.h config.h
