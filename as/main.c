
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

int
cmp(const void *f1, const void *f2)
{
	const Ins *ins = f2;

	return strcmp(f1, ins->str);
}

int
match(Op *op, Arg *args)
{
	return 1;
}

void
incpc(int siz)
{
	TUINT pc, curpc;
	pc = cursec->pc;
	curpc = cursec->curpc;

	cursec->curpc += siz;
	cursec->pc += siz;

	if (pass == 2)
		return;

	if (cursec->pc > cursec->max)
		cursec->max = cursec->pc;

	if (pc > cursec->pc ||
	    curpc > cursec->curpc ||
	    cursec->curpc > maxaddr ||
	    cursec->pc > maxaddr) {
		die("address overflow");
	}
}

void
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

int
dopass(char *fname)
{
	struct line line;
	FILE *fp;
	extern int nerrors;

	if ((fp = fopen(fname, "r")) == NULL)
		die("as: error opening '%s'", fname);

	isections();
	while (next(fp, &line))
		as(line.op, line.args);

	if (fclose(fp))
		die("as: error reading from input file '%s'", fname);
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
	for (pass = 1; pass <= 2 && dopass(argv[1]); pass++)
		/* nothing */;
	writeout("a.out");

	return 0;
}
