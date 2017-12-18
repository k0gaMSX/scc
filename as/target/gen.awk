
BEGIN		{
		FS = "\t"
		printf "#include \"../../../inc/scc.h\"\n"\
		       "#include \"../../as.h\"\n"\
		       "#include \"../" family "/proc.h\"\n"
		nop = 0; nvar = 0
}
		{sub(/#.*/,"")}

$6 !~ cpu	{next}

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
				printf "\t\t.bytes = (unsigned char []) {%s},\n",
				        opbytes[i]

			a = str2args(opargs[i])
			if (a != "")
				printf "\t\t.args = (unsigned char []) {%s}\n", a

			print "\t},"
		}
		print "};"
}

function str2args(s, args, i, out, n)
{
	n = split(s, args, /,/)
	if (n == 0 || args[1] == "none")
		return ""
	for (i = 1; i <= n; i++) {
		a = args[i]
		if (match(a, /^imm8/)) {
			out = out "AIMM8"
		} else if (match(a, /^imm16/)) {
			out = out "AIMM16"
		} else if (match(a, /^imm32/)) {
			out = out "AIMM32"
		} else if (match(a, /^imm64/)) {
			out = out "AIMM64"
		} else if (match(a, /^imm3/)) {
			out = out "AIMM3"
		} else if (match(a, /^rst/)) {
			out = out "ARST"
		} else if (match(a, /^\(IY\+n\)/)) {
			out = out "AINDEX_IY"
		} else if (match(a, /^\(IX\+n\)/)) {
			out = out "AINDEX_IX"
		} else if (match(a, /^ss/)) {
			out = out "AREG_SSCLASS"
		} else if (match(a, /^cc/)) {
			out = out "AREG_CCCLASS"
		} else if (match(a, /^dd/)) {
			out = out "AREG_DDCLASS"
		} else if (match(a, /^qq/)) {
			out = out "AREG_QQCLASS"
		} else if (match(a, /^rr/)) {
			out = out "AREG_RRCLASS"
		} else if (match(a, /^pp/)) {
			out = out "AREG_PPCLASS"
		} else if (match(a, /^p/)) {
			out = out "AREG_PCLASS"
		} else if (match(a, /^q/)) {
			out = out "AREG_QCLASS"
		} else if (match(a, /^r/)) {
			out = out "AREG_RCLASS"
		} else if (match(a, /^R/)) {
			out = out "AREG_R"
		} else if (match(a, /^\(DE\)/)) {
			out = out "AINDER_DE"
		} else if (match(a, /^\(BC\)/)) {
			out = out "AINDER_BC"
		} else if (match(a, /^\(HL\)/)) {
			out = out "AINDER_HL"
		} else if (match(a, /^\(SP\)/)) {
			out = out "AINDER_SP"
		} else if (match(a, /^\(C\)/)) {
			out = out "AINDER_C"
		} else if (match(a, /^SP/)) {
			out = out "AREG_SP"
		} else if (match(a, /^HL/)) {
			out = out "AREG_HL"
		} else if (match(a, /^DE/)) {
			out = out "AREG_DE"
		} else if (match(a, /^IX/)) {
			out = out "AREG_IX"
		} else if (match(a, /^IY/)) {
			out = out "AREG_IY"
		} else if (match(a, /^AF'/)) {
			out = out "AREG_AF_"
		} else if (match(a, /^AF/)) {
			out = out "AREG_AF"
		} else if (match(a, /^A/)) {
			out = out "AREG_A"
		} else if (match(a, /^F/)) {
			out = out "AREG_F"
		} else if (match(a, /^\(n\)/)) {
			out = out "ADIRECT"
		} else if (match(a, /^I/)) {
			out = out "AREG_I"
		} else if (match(a, /^0/)) {
			out = out "AZERO"
		} else if (match(a, /^sym/)) {
			out = out "ASYM"
		} else if (match(a, /^string/)) {
			out = out "ASTR"
		} else {
			print FILENAME ":" NR ":" $0 ":wrong arg", a > "/dev/stderr"
			exit 1
		}
		a = substr(a, RLENGTH+1)
		if (a ~ /^\+$/) {
			return out "|AREP"
		} else if (a ~ /^\?$/) {
			return out "|AOPT"
		} else if (a != "") {
			print FILENAME ":" NR ":" $0 ":wrong arg", a > "/dev/stderr"
			exit 1
		}
		out = out ","
	}
	out = out "0"

	return out
}
