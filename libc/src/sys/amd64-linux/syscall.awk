/^#/	{next}
	{name=$2 ".s"
	 printf "%s:\n\tmov\t%d,%%rax\n\tsyscall\n\tret\n",$2, $1 > name
	 close(name)}
