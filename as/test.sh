#!/bin/sh

set -e
exec >test.log
exec 2>&1

tmp1=`mktemp`
tmp2=`mktemp`

trap "rm -f a.out $tmp1 $tmp2" 0 2 3 

./as-$cpu target/$cpu/test.s

sed -n '/^\// ! {
	s%.*/%%
	s%^[ 	]*%%
	s%[ 	]*$%%
	/^$/d
	s%[ 	][ 	]*%\
%g
	p
}' target/$cpu/test.s |
nl -b a > $tmp1


../objdump/objdump |
sed -n '/^data:/,$ {
	/^data:/ ! {
		s%.*:%%
		s%^[ 	]*%%
		s%[ 	]*$%%
		/^$/d
		s%[ 	][ 	]*%\
%g
		p
	}
}' |
nl -b a > $tmp2

printf "test.s\n"
cat $tmp1
printf "\nobjdump\n"
cat $tmp2
printf "\ndiff\n"
diff $tmp1 $tmp2
