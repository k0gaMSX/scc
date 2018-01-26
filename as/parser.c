static char sccsid[] = "@(#) ./as/parser.c";
#include <ctype.h>
#include <limits.h>
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
		if ((*ap = operand(&s)) == NULL)
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

static void
comment(FILE *fp)
{
	int c;

	while ((c = getc(fp)) != EOF) {
		if (c != '*')
			continue;
		if ((c = getc(fp)) == '/')
			return;
		ungetc(c, fp);
	}
}

static size_t
getline(FILE *fp, char buff[MAXLINE])
{
	int c;
	char *bp;

	for (bp = buff; (c = getc(fp)) != EOF; *bp++ = c) {
		if (c == '\n')
			break;
		if (c == '/') {
			if ((c = getc(fp)) != '*') {
				ungetc(c, fp);
			} else {
				comment(fp);
				c = ' ';
			}
		} else if (c > UCHAR_MAX) {
			error("invalid character '%x'", c);
		}
		if (bp == &buff[MAXLINE])
			error("line too long");
	}
	return bp - buff;
}

int
nextline(FILE *fp, struct line *lp)
{
	size_t n;
	static char buff[MAXLINE];

repeat:
	if (feof(fp))
		return 0;
	if ((n = getline(fp, buff)) == 0)
		goto repeat;
	if (++lineno == 0)
		die("as: file too long");
	if (extract(buff, n, lp) == 0)
		goto repeat;
	return 1;
}
