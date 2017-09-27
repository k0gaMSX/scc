
BEGIN		{
		FS = "\t"
		printf "#include \"../../../inc/scc.h\"\n"\
		       "#include \"../../as.h\"\n"\
		       "#include \"../x86/proc.h\"\n"\
		       "#include \"ins.h\"\n\n"
		nop = 0; nvar = 0
}
		{sub(/#.*/,"")}

$6 !~ bits	{next}

/^$/		{next}

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
		formats[$5] = 1
}
END		{
		for (i in formats)
			printf "Format %s;\n", i

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
			       "\t\t.size = %d,\n"\
			       "\t\t.format = %s,\n",
			       opsize[i], opformat[i]

			if (opbytes[i] != "")
				printf "\t\t.bytes = (char []) {%s},\n",
				        opbytes[i]

			a = str2args(opargs[i])
			if (a != "")
				printf "\t\t.args = (char []) {%s}\n", a

			print "\t},"
		}
		print "};"
}

function str2args(s, args, i, out)
{
	if (split(s, args, /,/) == 0 || args[1] == "none")
		return ""
	for (i in args) {
		a = args[i]
		if (match(a, /^imm8/)) {
			out = "AIMM8"
		} else if (match(a, /^imm16/)) {
			out = "AIMM16"
		} else if (match(a, /^imm32/)) {
			out = "AIMM32"
		} else if (match(a, /^imm64/)) {
			out = "AIMM64"
		} else {
			print "wrong arg", a
			exit 1
		}
		a = substr(a, RLENGTH+1)
		if (a ~ /^\+$/) {
			return out "|AREP"
		} else if (a != "") {
			print "wrong arg", a
			exit 1
		}
		out = out ","
	}
	out = out "0"

	return out
}
