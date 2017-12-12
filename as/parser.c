static char sccsid[] = "@(#) ./as/parser.c";
#include <ctype.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define NARGS 20
#define MAXLINE 100

int nerrors;
jmp_buf recover;

static unsigned lineno;

void
error(char *msg, ...)
{
	va_list va;

	va_start(va, msg);
	fprintf(stderr, "as:%s:%u: ", infile, lineno);
	vfprintf(stderr, msg, va);
	putc('\n', stderr);
	nerrors++;
	va_end(va);

	if (nerrors == 10)
		die("as: too many errors");
	longjmp(recover, 1);
}

Node **
getargs(char *s)
{
	Node **ap;
	static Node *args[NARGS];

	if (!s)
		return NULL;

	for (ap = args; ap < &args[NARGS-1]; ++ap) {
		if ((*ap = expr(&s)) == NULL)
			return args;
	}
	error("too many arguments in one instruction");
}

static char *
field(char **oldp, size_t *siz)
{
	char *s, *begin;
	int c;

	if ((begin = *oldp) == NULL)
		return NULL;

	if (*begin == '/') {
		*begin = '\0';
		*oldp = NULL;
	} if (s = memchr(begin, '\t', *siz)) {
		*s++ = '\0';
		*siz -= s - begin;
		*oldp = s;
	} else {
		*oldp = NULL;
	}

	return (*begin != '\0') ? begin : NULL;
}

static int
validlabel(char *name)
{
	int c;

	while ((c = *name++) != '\0') {
		if (isalnum(c))
			continue;
		switch (c) {
		case '_':
		case '-':
		case '.':
		case '$':
			continue;
		default:
			return 0;
		}
	}
	return 1;
}

static int
extract(char *s, size_t len, struct line *lp)
{
	int r = 0;

	if (lp->label = field(&s, &len)) {
		size_t n = strlen(lp->label);
		if (lp->label[n-1] == ':')
			lp->label[n-1] = '\0';
		if (!validlabel(lp->label))
			error("incorrect label name '%s'", lp->label);
		r++;
	}
	if (lp->op = field(&s, &len))
		r++;
	if (lp->args = field(&s, &len))
		r++;

	if (s) {
		while (isspace(*s))
			++s;
		if (*s != '\0' && *s != '/')
			error("trailing characters at the end of the line");
	}

	return r;
}

int
nextline(FILE *fp, struct line *lp)
{
	size_t n;
	char *p;
	static char buff[MAXLINE];

repeat:
	if (!fgets(buff, sizeof(buff), fp))
		return 0;

	if (++lineno == 0)
		die("as: file too long");

	n = strlen(buff);
	if (n == 0)
		goto repeat;
	if (buff[n-1] != '\n') {
		error("line too long");
		goto repeat;
	}
	buff[n-1] = '\0';

	if (extract(buff, n, lp) == 0)
		goto repeat;
	return 1;
}
