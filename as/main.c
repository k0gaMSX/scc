static char sccsid[] = "@(#) ./as/main.c";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

int
match(Op *op, Arg *args)
{
	return 1;
}

static int
cmp(const void *f1, const void *f2)
{
	const Ins *ins = f2;

	return strcmp(f1, ins->str);
}

static void
as(char *text, char *xargs)
{
	Ins *ins;
	Op *op, *lim;
	Arg *args;
	
	ins = bsearch(text, instab, nr_ins, sizeof(Ins), cmp);
	if (!ins) {
		error("invalid instruction");
		return;
	}

	args = getargs(xargs);
	lim = &optab[ins->end];
	for (op = &optab[ins->begin]; op < lim; ++op) {
		if (match(op, args))
			break;
	}
	if (op == lim) {
		error("invalid operands");
		return;
	}
	(*op->format)(op, args);
}

static int
dopass(char *fname)
{
	struct line line;
	FILE *fp;
	extern int nerrors;

	if ((fp = fopen(fname, "r")) == NULL)
		die("as: error opening '%s'", fname);

	isections();
	while (nextline(fp, &line)) {
		linesym = NULL;

		if (line.label)
			linesym = deflabel(line.label);

		if (line.op)
			as(line.op, line.args);
		else if (line.args)
			error("arguments without an opcode");
	}

	if (fclose(fp))
		die("as: error reading from input file '%s'", fname);
	if (pass == 2)
		writeout("a.out");
	/*
	 * kill tmp symbols because they are not needed anymore
	 */
	killtmp();

	return nerrors == 0;
}

static void
usage(void)
{
	fputs("usage: as filename\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	if (argc != 2)
		usage();

	filename = argv[1];
	for (pass = 1; pass <= 2; pass++) {
		if (!dopass(filename))
			return 1;
	}

	return 0;
}
