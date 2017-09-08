
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define MAXLINE 100
int nerrors;

void
error(char *msg, ...)
{
	va_list va;

	va_start(va, msg);
	fputs("as: ", stderr);
	vfprintf(stderr, msg, va);
	putc('\n', stderr);
	nerrors++;
	va_end(va);
}

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

Arg *
getargs(char *text)
{
	return NULL;
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
	cursec->pc += op->size;
	cursec->curpc += op->size;
}

int
extract(char *p, char **lbl, char **ins, char **args)
{
	*lbl = p;
	*ins = NULL;
	*args = NULL;

	if ((p = strchr(p, '\t')) == NULL)
		return 0;
	*p++ = '\0';

	*ins = p;
	if ((p = strchr(p, '\t')) == NULL)
		return 1;
	*p++ = '\0';

	*args = p;
	if ((p = strchr(p, '\t')) == NULL)
		return 2;
	*p = '\0';
	return 3;
}

int
dopass(char *file)
{
	FILE *in;
	char line[MAXLINE], *ins, *lbl, *args;

	if ((in = fopen(file, "r")) == NULL)
		die("error opening '%s'", file);

	isections();

	while (fgets(line, sizeof(line), in)) {
		size_t n = strlen(line);
		if (n == 0 || line[0] == ';')
			continue;
		if (line[n-1] != '\n') {
			error("line too long");
			continue;
		}
		line[n-1] = '\0';
		if (extract(line, &lbl, &ins, &args) > 0)
			as(ins, args);
	}

	if (fclose(in))
		die("error reading from input file");
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

	for (pass = 1; pass <= 2 && dopass(argv[1]); pass++)
		/* nothing */;
	writeout("a.out");

	return 0;
}
