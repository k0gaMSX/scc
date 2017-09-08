
BEGIN	{
	FS = "\t"
	printf "#include \"../../../inc/scc.h\"\n"\
	       "#include \"../../as.h\"\n"\
	       "#include \"ins.h\"\n\n"
	nop = 0; nvar = 0
}
	{sub(/#.*/,"")}
/^$/	{next}
	{
	if (opstart[$1] == 0) {
		opstart[$1] = nvar
		opnames[nop++] = $1
	}
	opcount[$1]++
	opargs[nvar] = $2
	opsize[nvar] = $3
	opbytes[nvar] = $4
	opformat[nvar++] = $5
}
END	{
	printf "int nr_ins = %d;\n\n", nop
	print "struct ins instab[] = {"
	for (i = 0; i < nop; i++) {
		n = opnames[i]
		start = opstart[n]
		end = start + opcount[n]
		printf "\t{.str = \"%s\", .begin = %d, .end = %d},\n",
		       n, start, end | "sort"
	}
	close("sort")
	printf "};\n\n"

	print "struct op optab[] = {"
	for (i = 0; i < nvar; i++) {
		printf "\t{\n" \
		       "\t\t.bytes = (char []) {%s},\n"\
		       "\t\t.size = %d,\n"\
		       "\t\t.format = %s,\n"\
		       "\t\t.args = \"%s\"\n"\
		       "\t},\n",
		 opbytes[i], opsize[i], opformat[i], str2args(opargs[i])
	}
	print "};"
}

function str2args(s)
{
	return ""
}
