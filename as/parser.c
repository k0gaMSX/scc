static char sccsid[] = "@(#) ./as/parser.c";
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define NARGS 20
#define MAXLINE 100

char *filename;
int nerrors;

static unsigned lineno;

void
error(char *msg, ...)
{
	va_list va;

	va_start(va, msg);
	fprintf(stderr, "as:%s:%u: ", filename, lineno);
	vfprintf(stderr, msg, va);
	putc('\n', stderr);
	nerrors++;
	va_end(va);

	if (nerrors == 10)
		die("as: too many errors");
}

Node **
getargs(char *s)
{
	char *t;
	int ch, len;
	Node **ap;
	static Node *args[NARGS];

	if (!s)
		return NULL;

	for (ap = args; ; *ap++ = expr(t)) {
		while (isspace(*s))
			++s;
		if (*s == '\0')
			break;
		if (ap == &args[NARGS-1])
			error("too many arguments in one instruction");

		for (t = s; *s && *s != ','; s++)
			/* nothing */;
		len = t - s;
		if (*s != '\0')
			*s++ = '\0';
		if (len == 0)
			error("wrong operand '%s'", t);
	}
	*ap = NULL;

	return args;
}

static char *
field(char **oldp)
{
	char *s, *begin;
	int c;

	if ((begin = *oldp) == NULL)
		return NULL;

	for (s = begin; ; s++) {
		switch (*s) {
		case '\0':
			*oldp = NULL;
			goto out_loop;
		case '\t':
		case ';':
			*s = '\0';
			*oldp = s+1;
			goto out_loop;
		case '\'':
			if (*++s == '\0' || *++s != '\'')
				error("invalid character constant");
			break;
		case '"':
			while ((c = *++s) && c != '"')
				/* nothing */;
			if (c == '\0')
				error("unterminated string");
			break;
		default:
			*s = toupper(*s);
			break;
		}
	}

out_loop:
	return (*begin != '\0') ? begin : NULL;
}

static int
extract(char *s, struct line *lp)
{
	int r = 0;

	if (lp->label = field(&s))
		r++;
	if (lp->op = field(&s))
		r++;
	if (lp->args = field(&s))
		r++;

	if (s) {
		while (isspace(*s))
			++s;
		if (*s != '\0' && *s != ';')
			error("trailing characters at the end of the line");
	}

	return r;
}

int
nextline(FILE *fp, struct line *lp)
{
	size_t n;
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
	if (extract(buff, lp) == 0)
		goto repeat;
	return 1;
}
