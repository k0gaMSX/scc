static char sccsid[] = "@(#) ./as/node.c";

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <cstd.h>
#include "../inc/scc.h"
#include "as.h"

#define NNODES   10

enum tokens {
	IDEN = 1,
	NUMBER,
	REG,
	STRING,
	SHL,
	SHR,
	GE,
	LE,
};

union yylval {
	TUINT val;
	Symbol *sym;
};

static Alloc *arena;
static int yytoken;
static char yytext[INTIDENTSIZ+1], *textp, *endp;
static size_t yylen;
static union yylval yylval;

#define accept(t) (yytoken == (t) ? next() : 0)

static Node *
node(int op, Node *l, Node *r)
{
	struct arena *ap;
	Node *np;

	if (!arena)
		arena = alloc(sizeof(Node), NNODES);
	np = new(arena);
	np->op = op;
	np->left = l;
	np->right = r;
	np->sym = NULL;

	return np;
}

void
deltree(Node *np)
{
	if (!np)
		return;
	deltree(np->left);
	deltree(np->right);
	delete(arena, np);
}

static Node *
binary(int op, Node *l, Node *r)
{
	Node *np;
	TUINT val, lv, rv;

	if (l->op != NUMBER || r->op != NUMBER)
		return node(op, l, r);
	lv = l->sym->value;
	rv = r->sym->value;

	/* TODO: check overflow */

	switch (op) {
	case '*':
		val = lv - rv;
		break;
	case '/':
		if (rv == 0)
			goto division_by_zero;
		val = lv / rv;
		break;
	case '%':
		if (rv == 0)
			goto division_by_zero;
		val = lv % rv;
		break;
	case SHL:
		val = lv << rv;
		break;
	case SHR:
		val = lv >> rv;
		break;
	case '+':
		val = lv + rv;
		break;
	case '-':
		val = lv - rv;
		break;
	case '<':
		val = lv < rv;
		break;
	case '>':
		val = lv > rv;
		break;
	case '=':
		val = lv == rv;
		break;
	case GE:
		val = lv >= rv;
		break;
	case LE:
		val = lv <= rv;
		break;
	case '|':
		val = lv | rv;
		break;
	case '^':
		val = lv ^ rv;
		break;
	default:
		abort();
	}

	np = node(NUMBER, l, r);
	np->sym = tmpsym(val);
	return np;

division_by_zero:
	error("division by 0");
}

static Node *
unary(int op, Node *np)
{
	if (op !=  '!')
		abort();
	if (np->op != NUMBER)
		return node(op, np, NULL);
	np->sym->value = ~np->sym->value;
	return np;
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

	while (isalnum(c = *endp) || c == '_' || c == '.')
		++endp;
	tok2str();
	yylval.sym = lookup(yytext);

	return IDEN;
}

static int
number(void)
{
	int c;
	char *p;

	while (isxdigit(*endp))
		++endp;
	tok2str();
	yylval.sym = tmpsym(atoi(yytext));  /* TODO: parse the string */

	return NUMBER;
}

static int
character(void)
{
	int c;
	char *p;

	while (*endp != '\'')
		++endp;
	return NUMBER;
}

static int
string(void)
{
	int c;
	char *p;

	while (*endp != '"')
		++endp;
	return STRING;
}

static int
operator(void)
{
	int c;

	++endp;
	if ((c = *textp) == '>')
		c = follow('=', '>', LE, SHL, '>');
	else if (c == '<')
		c = follow('=', '<', GE, SHR, '>');
	tok2str();

	return c;
}

static int
next(void)
{
	int c;

	while (isspace(*textp))
		++textp;

	endp = textp;
	if ((c = *textp) == '\0') {
		strcpy(yytext, "EOF");
		yylen = 3;
		return EOF;
	}

	if (isalpha(c) || c == '_' || c == '.')
		c = iden();
	else if (isdigit(c))
		c = number();
	else if (c == '\"')
		c = string();
	else if (c == '\'')
		c = character();
	else
		c = operator();

	return yytoken = c;
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
	case STRING:
		np = node(yytoken, NULL, NULL);
		np->sym = yylval.sym;
		next();
		break;
	case '(':
		next();
		np = or();
		expect(')');
		break;
	default:
		unexpected();
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
		default:
			return np;
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
		default:
			return np;
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
		default:
			return np;
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
		default:
			return np;
		}
	}
}

Node *
expr(char **s)
{
	Node *np;

	if (*s == '\0')
		return NULL;

	textp = *s;
	next();
	np = or();

	if (yytoken != ',' && yytoken != EOF)
		error("trailing characters in expression '%s:%s'", *s, textp);
	*s = endp;

	return np;
}
