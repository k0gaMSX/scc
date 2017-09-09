#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define NARGS 20
#define MAXLINE 100


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

int
extract(char *p, struct line *linep)
{
	linep->label = p;
	linep->op = NULL;
	linep->args = NULL;

	if ((p = strchr(p, '\t')) == NULL)
		return 0;
	*p++ = '\0';

	linep->op = p;
	if ((p = strchr(p, '\t')) == NULL)
		return 1;
	*p++ = '\0';

	linep->args = p;
	if ((p = strchr(p, '\t')) == NULL)
		return 2;
	*p = '\0';
	return 3;
}

int
next(FILE *fp, struct line *linep)
{
	size_t n;
	static char buff[MAXLINE];

repeat:
	if (!fgets(buff, sizeof(buff), fp))
		return 0;

	n = strlen(buff);
	if (n == 0 || buff[0] == ';')
		goto repeat;
	if (buff[n-1] != '\n') {
		error("buff too long");
		goto repeat;
	}
	buff[n-1] = '\0';
	if (extract(buff, linep) == 0)
		goto repeat;
}
