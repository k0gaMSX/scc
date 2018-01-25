static char sccsid[] = "@(#) ./as/node.c";

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <cstd.h>
#include "../inc/scc.h"
#include "as.h"

#define NNODES   10

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
fold(int op, Node *l, Node *r)
{
	Node *np;
	TUINT val, lv, rv;

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
	deltree(l);
	deltree(r);

	np = node(NUMBER, NULL, NULL);
	np->sym = tmpsym(val);
	np->addr = ANUMBER;
	return np;

division_by_zero:
	error("division by 0");
}

static Node *
binary(int op, Node *l, Node *r)
{
	int addr;
	Node *np;

	if (l->op == NUMBER && r->op == NUMBER)
		return fold(op, l, r);
	else if (l->addr == AREG && r->addr == ANUMBER)
		addr = AREG_OFF;
	else if (l->addr == AREG && l->addr != ANUMBER)
		error("incorrect operand");
	np = node(op, l, r);
	np->addr = addr;

	return np;
}

static Node *
unaryop(int op, Node *np)
{
	TUINT val;

	if (np->addr != ANUMBER)
		error("invalid argument for unary operator");
	if (np->op != NUMBER) {
		np = node(op, np, NULL);
		np->addr = ANUMBER;
		return np;
	}

	val = np->sym->value;
	switch (op) {
	case '!':
		val = !val;
	case '+':
		break;
	case '-':
		val = -val;
		break;
	default:
		abort();
	}
	np->sym->value = val;

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

	for ( ; c = *endp; ++endp) {
		if (isalnum(c))
			continue;
		switch (c) {
		case '\'':
		case '_':
		case '-':
		case '.':
		case '$':
			continue;
		default:
			goto out_loop;
		}
	}

out_loop:
	tok2str();
	yylval.sym = lookup(yytext);

	return ((yylval.sym->flags & FTMASK) == FREG) ? REG : IDEN;
}

static int
number(void)
{
	int c, base = 10;
	char *p;
	TUINT n;

	if (*endp == '0') {
		base = 8;
		++endp;
		if (*endp == 'x') {
			base = 16;
			++endp;
		}
	}
	for (n = 0; (c = *endp) && isxdigit(c); n += c) {
		n *= base;
		c -= '0';
		if (n >= TUINT_MAX - c*base)
			error("overflow in number");
		endp++;
	}
	tok2str();
	yylval.sym = tmpsym(n);

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

	for (++endp; *endp != '"'; ++endp)
		;
	++endp;
	tok2str();
	yylval.sym = tmpsym(0);
	/* FIXME: this memory is not freed ever */
	yylval.sym->name.buf = xstrdup(yytext);

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

	switch (c = *textp) {
	case '\0':
		strcpy(yytext, "EOS");
		yylen = 3;
		c = EOS;
		break;
	case '"':
		c = string();
		break;
	case '\'':
		c = character();
		break;
	case '_':
		c = iden();
		break;
	default:
		if (isdigit(c))
			c = number();
		else if (isalpha(c))
			c = iden();
		else
			c = operator();
		break;
	}
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

static Node *expr(void);

Node *
zilog(void)
{
	int op;
	Node *np = expr();

	switch (np->addr) {
	case AREG:
		op = AINDIR;
		break;
	case AREG_OFF:
		op = AINDEX;
		break;
	case ANUMBER:
		op = ADIRECT;
		break;
	default:
		abort();
	}
	np = node(op, np, NULL);
	np->addr = op;
	return np;
}

/*************************************************************************/
/* grammar functions                                                     */
/*************************************************************************/

static Node *
primary(void)
{
	int addr, op;
	Node *np;

	switch (yytoken) {
	case REG:
		addr = AREG;
		goto basic_atom;
	case IDEN:
	case NUMBER:
		addr = ANUMBER;
		goto basic_atom;
	case STRING:
		addr = ASTR;
	basic_atom:
		np = node(yytoken, NULL, NULL);
		np->sym = yylval.sym;
		np->addr = addr;
		next();
		break;
	case '(':
		np = expr();
		expect(')');
		break;
	default:
		unexpected();
	}

	return np;
}

static Node *
unary(void)
{
	int op, tok;
	Node *np;

	switch (tok = yytoken) {
	case '%':
	case '$':
	case '.':
		/* TODO: implement identifiers with %, $ and . */
	case '!':
	case '-':
	case '+':
		next();
		return unaryop(tok, primary());
	default:
		return primary();
	}
}

static Node *
mul(void)
{
	int op;
	Node *np;

	np = unary();
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
and(void)
{
	int op;
	Node *np;

	np = relational();
	while (accept('&'))
		np = binary('&', np, relational());
	return np;
}

static Node *
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
		default:
			return np;
		}
	}
}

Node *
operand(char **strp)
{
	int imm = 0;
	Node *np;

	textp = *strp;
	switch (next()) {
	case EOS:
		np = NULL;
		break;
	case '(':
		next();
		np = zilog();
		expect(')');
		break;
	case REG:
		np = node(yytoken, NULL, NULL);
		np->sym = yylval.sym;
		np->addr = AREG;
		next();
		break;
	case '$':
		next();
		imm = 1;
	default:
		np = expr();
		if (imm)
			np->addr = AIMM;
	}
	if (yytoken != ',' && yytoken != EOS)
		error("trailing characters in expression '%s'", textp);
	*strp = endp;

	return np;
}
