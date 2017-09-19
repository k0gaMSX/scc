static char sccsid[] = "@(#) ./as/node.c";

#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define NNODES   10

enum tokens {
	IDEN = 1,
	CHAR,
	STRING,
	SHL,
	SHR,
	GE,
	LT,
};

static Alloc *arena;
static int yytoken;
static char *yytext;

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
next(void)
{
	return 0;
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

static Node *
primary(void)
{
	Node *np;

	switch (yytoken) {
	case IDEN:
	case CHAR:
	case STRING:
		np = node(yytoken, NULL, NULL);
		next();
		break;
	case '(':
		next();
		np = expr();
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
		case GE:
		case LT:
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

Node *
expr(void)
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
