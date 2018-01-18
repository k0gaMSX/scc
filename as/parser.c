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
	char *s, *t, *begin;
	size_t n;

	if ((begin = *oldp) == NULL)
		return NULL;

	for (s = begin; *s == ' '; ++s)
		;
	if (*s == '\0' || *s == '/' || *s == ';') {
		*t = '\0';
		return *oldp = NULL;
	}

	for (t = s; *t && *t != '\t'; ++t)
		;
	if (*t == '\t')
		*t++ = '\0';
	*siz -= begin - t;
	*oldp = t;

	while (t >= s && *t == ' ')
		*t-- = '\0';
	return (*s != '\0') ? s : NULL;
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
		case ':':
			if (*name != '\0')
				return 0;
			*--name = '\0';
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

	if (lp->label = field(&s, &len))
		r++;
	if (lp->op = field(&s, &len))
		r++;
	if (lp->args = field(&s, &len))
		r++;

	if (s && *s && *s != '/')
		error("trailing characters at the end of the line");
	if (lp->label && !validlabel(lp->label))
		error("incorrect label name '%s'", lp->label);

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
	if (buff[n-1] != '\n')
		error("line too long");
	buff[n-1] = '\0';

	if (extract(buff, n, lp) == 0)
		goto repeat;
	return 1;
}
