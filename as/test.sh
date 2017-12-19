#!/bin/sh

set -e

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f a.out $tmp1 $tmp2" 0 2 3 

./as-$cpu target/$cpu/test.s

sed -n '/^\// ! {
	s%.*/ %%
	s%  *%\
/%
	w '$tmp1'
}' target/$cpu/test.s


../objdump/objdump |
sed -n '/^data:/,$ {
	/^data:/ ! {
		s%.*: %%
		s%  *%\
%g
		w '$tmp2'
	}
}' 

diff $tmp1 $tmp2
