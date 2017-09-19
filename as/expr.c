static char sccsid[] = "@(#) ./as/node.c";

#include <ctype.h>
#include <string.h>

#include <cstd.h>
#include "../inc/scc.h"
#include "as.h"

#define NNODES   10

enum tokens {
	IDEN = 1,
	NUMBER,
	REG,
	CHAR,
	STRING,
	SHL,
	SHR,
	GE,
	LE,
};

static Alloc *arena;
static int yytoken;
static char yytext[INTIDENTSIZ+1], *textp, *endp;
static size_t yylen;

#define accept(t) (yytoken == (t) ? next() : 0)

static Node *
node(int op, Node *l, Node *r)
{
	struct arena *ap;
	Node *np;

	if (!arena)
		arena = alloc(sizeof(Node), NNODES);
	np = memset(new(arena), 0, sizeof(*np));
	np->op = op;

	return np;
}

static Node *
binary(int op, Node *l, Node *r)
{
	return node(op, l, r);
}

static Node *
unary(int op, Node *np)
{
	return node(op, np, NULL);
}

static int
follow(int expect1, int expect2, int ifyes1, int ifyes2, int ifno)
{
	int c;

	if ((c = *++textp) == expect1)
		return ifyes1;
	if (c == expect2)
		return ifyes2;
	--textp;
	return ifno;
}

static void
tok2str(void)
{
	if ((yylen = endp - textp) > INTIDENTSIZ) {
		error("token too big");
		yylen = INTIDENTSIZ;
	}
	memcpy(yytext, textp, yylen);
	yytext[yylen] = '\0';
	textp = endp;
}

static int
iden(void)
{
	int c;
	char *p;

	for (endp = textp; isalnum(c = *endp) || c == '_' || c == '.'; ++endp)
		/* nothing */;
	return IDEN;
}

static int
number(void)
{
	int c;
	char *p;

	for (endp = textp; isxdigit(*endp); ++endp)
		/* nothing */;
	return NUMBER;
}

static int
character(void)
{
	int c;
	char *p;

	for (endp = textp+1; *endp != '\''; ++endp)
		/* nothing */;
	return CHAR;
}

static int
string(void)
{
	int c;
	char *p;

	for (endp = textp+1; *endp != '"'; ++endp)
		/* nothing */;
	return STRING;
}

static int
next(void)
{
	int c;

	while (isspace(*textp))
		++textp;
	c = *textp;
	if (isalpha(c) || c == '_' || c == '.')
		c = iden();
	else if (isdigit(c))
		c = number();
	else if (c == '>')
		c = follow('=', '>', LE, SHL, '>');
	else if (c == '<')
		c = follow('=', '<', GE, SHR, '>');
	else if (c == '\'')
		c = character();
	else if (c == '\"')
		c = string();
	tok2str();

	return c;
}

static void
unexpected(void)
{
	error("unexpected '%s'", yytext);
}

static void
expect(int token)
{
	if (yytoken != token)
		unexpected();
	next();
}

/*************************************************************************/
/* grammar functions                                                     */
/*************************************************************************/

static Node *or(void);

static Node *
primary(void)
{
	Node *np;

	switch (yytoken) {
	case NUMBER:
	case IDEN:
	case CHAR:
	case STRING:
		np = node(yytoken, NULL, NULL);
		next();
		break;
	case '(':
		next();
		np = or();
		expect(')');
		break;
	default:
		unexpected();
		return NULL;
	}

	return np;
}

static Node *
mul(void)
{
	int op;
	Node *np;

	np = primary();
	for (;;) {
		switch (op = yytoken) {
		case '*':
		case '/':
		case '%':
		case SHL:
		case SHR:
			next();
			binary(op, np, primary());
			break;
		default: return np;
		}
	}
}

static Node *
add(void)
{
	int op;
	Node *np;

	np = mul();
	for (;;) {
		switch (op = yytoken) {
		case '+':
		case '-':
			next();
			np = binary(op, np, mul());
			break;
		default: return np;
		}
	}
}

static Node *
relational(void)
{
	int op;
	Node *np;

	np = add();
	for (;;) {
		switch (op = yytoken) {
		case '<':
		case '>':
		case '=':
		case GE:
		case LE:
			next();
			np = binary(op, np, add());
			break;
		default: return np;
		}
	}
}

static Node *
not(void)
{
	Node *np;

	np = relational();
	while (accept('!'))
		np = unary('!', relational());
	return np;
}

static Node *
and(void)
{
	int op;
	Node *np;

	np = not();
	while (accept('&'))
		np = binary('&', np, not());
	return np;
}

static Node *
or(void)
{
	int op;
	Node *np;

	np = and();
	for (;;) {
		switch (op = yytoken) {
		case '|':
		case '^':
			next();
			np = binary(op, np, and());
			break;
		default: return np;
		}
	}
}

Node *
expr(char *s)
{
	Node *np;

	textp = s;
	next();
	np = or();

	if (*textp != '\0')
		error("trailing characters in expression '%s'", textp);
	return np;
}
