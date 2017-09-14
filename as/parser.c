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

Arg
number(char *s, int base)
{
	Arg arg;
	TUINT n;

	/* TODO: Check overflow here */
	arg.type = AIMM;
	for (n = 0; *s; n += *s++ - '0')
		n *= base;
	arg.val = n;

	return arg;
}

Arg *
getargs(char *s)
{
	char *t;
	int ch, len;
	Arg *ap;
	static Arg args[NARGS];

	for (ap = args; s; ++ap) {
		while (isspace(*s))
			++s;
		if (*s == '\0')
			break;
		if (ap == &args[NARGS-1])
			die("too many arguments in one instruction");

		for (t = s; *s && *s != ','; s++)
			/* nothing */;
		len = t - s;
		if (len == 0)
			goto wrong_operand;

		if (*s)
			*s++ = '\0';

		ch = *t;
		if (isdigit(ch)) {
			*ap = number(t, (s[len-1] == 'H') ? 16 : 10);
			continue;
		}
wrong_operand:
		error("wrong operand '%s'", t);
	}
	ap->type = 0;

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
		switch (c = *s) {
		case '\t':
			*s++ = '\0';
			*oldp = s;
			goto out_loop;
		case ';':
			*s = '\0';
		case '\0':
			*oldp = NULL;
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
			*s = toupper(c);
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
next(FILE *fp, struct line *lp)
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
