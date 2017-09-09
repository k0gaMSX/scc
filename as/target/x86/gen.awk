
BEGIN	{
	FS = "\t"
	printf "#include \"../../../inc/scc.h\"\n"\
	       "#include \"../../as.h\"\n"\
	       "#include \"../../ins.h\"\n"\
	       "#include \"../x86/args.h\"\n"\
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
	opbytes[nvar] = ($4 == "none") ? "" : $4
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
		       "\t\t.args = (char []) {%s}\n"\
		       "\t},\n",
		 opbytes[i], opsize[i], opformat[i], str2args(opargs[i])
	}
	print "};"
}

function str2args(s, args, i, out)
{
	split(s, args, /,/)
	for (i in args) {
		if (args[i] == "none")
			break
		else if (args[i] ~ /imm8/)
			out = "AIMM8"
		else if (args[i] ~ /imm16/)
			out = "AIMM16"
		else if (args[i] ~ /imm32/)
			out = "AIMM32"
		else if (args[i] ~ /imm64/)
			out = "AIMM64"
		else {
			print "wrong arg", args[i]
			exit 1
		}
		if (args[i] ~ /\*$/)
			return out "|AREP"
		out = out ","
	}
	out = out "0"

	return out
}
