#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/cc.h"
#include "../inc/sizes.h"
#include "cc1.h"

Node *expr(void);

bool
cmpnode(Node *np, TUINT val)
{
	Symbol *sym;
	Type *tp;

	if (!np || !np->constant)
		return 0;
	sym = np->sym;
	tp = sym->type;

	switch (tp->op) {
	case PTR:
	case INT:
		return ((tp->sign) ? sym->u.i : sym->u.u) == val;
	case FLOAT:
		return sym->u.f == val;
	}
	return 0;
}

bool
isnodecmp(int op)
{
	switch (op) {
	case OEQ:
	case ONE:
	case OLT:
	case OGE:
	case OLE:
	case OGT:
		return 1;
	default:
		return 0;
	}
}

static Node *
promote(Node *np)
{
	Type *tp;
	Node *new;
	unsigned r;

	tp = np->type;
	r = tp->n.rank;
	if  (r > RANK_UINT || tp == inttype || tp == uinttype)
		return np;
	tp = (r == RANK_UINT) ? uinttype : inttype;
	if ((new = convert(np, tp, 1)) != NULL)
		return new;
	return np;
}

static void
typeconv(Node **p1, Node **p2)
{
	Type *tp1, *tp2;
	Node *np1, *np2;
	int n;

	np1 = promote(*p1);
	np2 = promote(*p2);

	tp1 = np1->type;
	tp2 = np2->type;
	if (tp1 != tp2) {
		if ((n = tp1->n.rank - tp2->n.rank) > 0)
			np2 = convert(np2, tp1, 1);
		else if (n < 0)
			np1 = convert(np1, tp2, 1);
	}
	*p1 = np1;
	*p2 = np2;
}

static void
chklvalue(Node *np, Type *tp)
{
	if (!np->lvalue)
		error("lvalue required in operation");
	if (np->type == voidtype)
		error("invalid use of void expression");
}

static Node *
decay(Node *np)
{
	Type *tp = np->type;

	switch (tp->op) {
	case ARY:
		tp = tp->type;
		if (np->op == OPTR) {
			Node *new = np->left;
			free(np);
			new->type = mktype(tp, PTR, 0, NULL);
			return new;
		}
	case FTN:
		np = node(OADDR, mktype(tp, PTR, 0, NULL), np, NULL);
	default:
		return np;
	}
}

Node *
eval(Node *np)
{
	Node *p;

	if (!np)
		return NULL;

	np = decay(np);
	if (!isnodecmp(np->op))
		return np;
	p = node(OCOLON, inttype, constnode(one), constnode(zero));
	return node(OASK, inttype, np, p);
}

static Node *
integerop(char op, Node *lp, Node *rp)
{
	lp = eval(lp);
	rp = eval(rp);
	if (BTYPE(lp) != INT || BTYPE(rp) != INT)
		error("operator requires integer operands");
	typeconv(&lp, &rp);
	return simplify(op, lp->type, lp, rp);
}

static Node *
numericaluop(char op, Node *np)
{
	np = eval(np);
	switch (BTYPE(np)) {
	case INT:
	case FLOAT:
		if (op == ONEG && np->op == ONEG)
			return np->left;
		if (op == OADD)
			return np;
		return simplify(op, np->type, np, NULL);
	default:
		error("unary operator requires numerical operand");
	}
}

static Node *
integeruop(char op, Node *np)
{
	np = eval(np);
	if (BTYPE(np) != INT)
		error("unary operator requires integer operand");
	if (op == OCPL && np->op == OCPL)
		return np->left;
	return simplify(op, np->type, np, NULL);
}

Node *
convert(Node *np, Type *newtp, char iscast)
{
	Type *oldtp = np->type;

	if (eqtype(newtp, oldtp))
		return np;

	switch (oldtp->op) {
	case ENUM:
	case INT:
	case FLOAT:
		switch (newtp->op) {
		case PTR:
			if (!iscast || oldtp->op == FLOAT)
				return NULL;
			/* PASSTHROUGH */
		case INT:
		case FLOAT:
		case ENUM:
		case VOID:
			break;
		default:
			return NULL;
		}
		break;
	case PTR:
		switch (newtp->op) {
		case ENUM:
		case INT:
		case VOID:
			if (!iscast)
				return NULL;
			break;
		case PTR:
			if (iscast ||
			    newtp == pvoidtype ||
			    oldtp == pvoidtype) {
				/* TODO:
				 * we assume conversion between pointers
				 * do not need any operation, but due to
				 * alignment problems that may be false
				 */
				np->type = newtp;
				return np;
			}
		default:
			return NULL;
		}
	default:
			return NULL;
	}
	return castcode(np, newtp);
}

static Node *
parithmetic(char op, Node *lp, Node *rp)
{
	Type *tp;
	Node *size;

	tp = lp->type;
	size = sizeofnode(tp->type);

	if (op == OSUB && BTYPE(rp) == PTR) {
		if (tp != rp->type)
			goto incorrect;
		lp = node(OSUB, inttype, lp, rp);
		return node(ODIV, inttype, lp, size);
	}
	if (BTYPE(rp) != INT)
		goto incorrect;

	rp = convert(promote(rp), sizettype, 0);
	rp = simplify(OMUL, sizettype, rp, size);
	rp = convert(rp, tp, 1);

	return simplify(OADD, tp, lp, rp);

incorrect:
	errorp("incorrect arithmetic operands");
	return node(OADD, tp, lp, rp);
}

static Node *
arithmetic(char op, Node *lp, Node *rp)
{
	lp = eval(lp);
	rp = eval(rp);
	switch (BTYPE(lp)) {
	case INT:
	case FLOAT:
		switch (BTYPE(rp)) {
		case INT:
		case FLOAT:
			typeconv(&lp, &rp);
			break;
		case PTR:
			if (op == OADD || op == OSUB)
				return parithmetic(op, rp, lp);
		default:
			goto incorrect;
		}
		break;
	case PTR:
		return parithmetic(op, lp, rp);
	default:
	incorrect:
		errorp("incorrect arithmetic operands");
	}

	return simplify(op, lp->type, lp, rp);
}

static Node *
pcompare(char op, Node *lp, Node *rp)
{
	switch (BTYPE(rp)) {
	case INT:
		if (cmpnode(rp, 0))
			rp = convert(rp, pvoidtype, 1);
		break;
	case PTR:
		if (lp->type != rp->type)
			warn("comparision between different pointer types");
		break;
	default:
		errorp("incompatibles type in comparision");
	}
	return simplify(op, inttype, lp, rp);
}

static Node *
compare(char op, Node *lp, Node *rp)
{
	lp = eval(lp);
	rp = eval(rp);
	switch (BTYPE(lp)) {
	case INT:
	case FLOAT:
		switch (BTYPE(lp)) {
		case INT:
		case FLOAT:
			typeconv(&lp, &rp);
			break;
		case PTR:
			return pcompare(op, rp, lp);
		default:
			goto nocompat;
		}
		break;
	case PTR:
		return pcompare(op, lp, rp);
	default:
	nocompat:
		errorp("incompatibles type in comparision");
	}

	return simplify(op, inttype, lp, rp);
}

int
negop(int op)
{
	switch (op) {
	case OAND: return OOR;
	case OOR:  return OAND;
	case OEQ:  return ONE;
	case ONE:  return OEQ;
	case OLT:  return OGE;
	case OGE:  return OLT;
	case OLE:  return OGT;
	case OGT:  return OLE;
	}
	return op;
}

Node *
negate(Node *np)
{
	np->op = negop(np->op);
	return np;
}

Node *
exp2cond(Node *np, char neg)
{
	np = decay(np);
	if (isnodecmp(np->op))
		return (neg) ? negate(np) : np;
	return compare((neg) ?  OEQ : ONE, np, constnode(zero));
}

static Node *
logic(char op, Node *lp, Node *rp)
{
	lp = exp2cond(lp, 0);
	rp = exp2cond(rp, 0);
	return simplify(op, inttype, lp, rp);
}

static Node *
field(Node *np)
{
	Symbol *sym;

	namespace = np->type->ns;
	next();
	namespace = NS_IDEN;

	sym = yylval.sym;
	if (yytoken != IDEN)
		unexpected();
	next();

	switch (BTYPE(np)) {
	case STRUCT:
	case UNION:
		if ((sym->flags & ISDECLARED) == 0)
			error("incorrect field in struct/union");
		np = node(OFIELD, sym->type, np, varnode(sym));
		np->lvalue = 1;
		return np;
	default:
		error("request for member '%s' in something not a structure or union",
		      yylval.sym->name);
	}
}

static Node *
content(char op, Node *np)
{
	switch (BTYPE(np)) {
	case ARY:
	case FTN:
	case PTR:
		if (np->op == OADDR) {
			Node *new = np->left;
			new->type = np->type->type;
			free(np);
			np = new;
		} else {
			np = node(op, np->type->type, np, NULL);
		}
		np->lvalue = 1;
		return np;
	default:
		error("invalid argument of memory indirection");
	}
}

static Node *
array(Node *lp, Node *rp)
{
	Type *tp;
	Node *np;

	if (BTYPE(lp) != INT && BTYPE(rp) != INT)
		error("array subscript is not an integer");
	np = arithmetic(OADD, lp, rp);
	tp = np->type;
	if (tp->op != PTR)
		errorp("subscripted value is neither array nor pointer");
	return content(OPTR, np);
}

static Node *
assignop(char op, Node *lp, Node *rp)
{
	lp = eval(lp);
	rp = eval(rp);

	if (BTYPE(rp) == INT && BTYPE(lp) == PTR && cmpnode(rp, 0))
		rp = convert(rp, pvoidtype, 1);
	else if ((rp = convert(rp, lp->type, 0)) == NULL)
		errorp("incompatible types when assigning");

	return node(op, lp->type, lp, rp);
}

static Node *
incdec(Node *np, char op)
{
	Type *tp = np->type;
	Node *inc;

	chklvalue(np, np->type);

	switch (BTYPE(np)) {
	case PTR:
		if (!tp->defined)
			error("invalid use of indefined type");
		inc = sizeofnode(tp->type);
		break;
	case INT:
	case FLOAT:
		inc = constnode(one);
		break;
	default:
		errorp("wrong type argument to increment or decrement");
		return np;
	}
	return arithmetic(op, np, inc);
}

static Node *
address(char op, Node *np)
{
	if (!np->lvalue)
		error("lvalue required in unary expression");
	if (np->symbol && (np->sym->flags & ISREGISTER))
		error("address of register variable '%s' requested", yytext);
	if (np->op == OPTR) {
		Node *new = np->left;
		free(np);
		return new;
	}
	return node(op, mktype(np->type, PTR, 0, NULL), np, NULL);
}

static Node *
negation(char op, Node *np)
{
	switch (BTYPE(np)) {
	case FTN:
	case ARY:
	case INT:
	case FLOAT:
	case PTR:
		return exp2cond(np, 1);
	default:
		error("invalid argument of unary '!'");
	}
}

/*************************************************************
 * grammar functions                                         *
 *************************************************************/
static Node *
primary(void)
{
	Node *np;
	Symbol *sym;

	switch (yytoken) {
	case CONSTANT:
		np = constnode(yylval.sym);
		next();
		break;
	case IDEN:
		sym = yylval.sym;
		if ((sym->flags & ISDECLARED) == 0) {
			errorp("'%s' undeclared", yytext);
			sym->type = inttype;
			install(sym->ns, yylval.sym);
		}
		sym->flags |= ISUSED;
		np = varnode(sym);
		next();
		break;
	default:
		unexpected();
	}
	return np;
}

static Node *assign(void);

static Node *
arguments(Node *np)
{
	int n;
	Node *par = NULL, *arg;
	Type **targs, *tp = np->type;

	if (tp->op == PTR && tp->type->op == FTN) {
		np = content(OPTR, np);
		tp = np->type;
	}
	if (tp->op != FTN)
		error("function or function pointer expected");
	targs = tp->p.pars;

	expect('(');

	if ((n = tp->n.elem) > 0) {
		do {
			if ((arg = eval(assign())) == NULL)
				unexpected();
			if ((arg = convert(arg, *targs++, 0)) == NULL)
				goto bad_type;
			par = node(OPAR, arg->type, par, arg);
		} while (--n && accept(','));
	}

	if (n > 0)
		error("too few arguments in function call");
	if (yytoken == ',')
		error("too many arguments in function call");

	expect(')');
	return node(OCALL, np->type->type, np, par);

bad_type:
	error("incompatible type for argument %d in function call",
	      tp->n.elem - n + 1);
}

static Node *
postfix(Node *lp)
{
	Node *rp;

	if (!lp)
		lp = primary();
	for (;;) {
		switch (yytoken) {
		case '[':
			next();
			rp = expr();
			lp = array(lp, rp);
			expect(']');
			break;
		case DEC:
		case INC:
			lp = incdec(lp, (yytoken == INC) ? OINC : ODEC);
			next();
			break;
		case INDIR:
			lp = content(OPTR, lp);
		case '.':
			lp = field(lp);
			break;
		case '(':
			lp = arguments(lp);
			break;
		default:
			return lp;
		}
	}
}

static Node *unary(void);

static Type *
typeof(Node *np)
{
	Type *tp;

	if (np == NULL)
		unexpected();
	tp = np->type;
	freetree(np);
	return tp;
}

static Type *
sizeexp(void)
{
	Type *tp;

	expect('(');
	switch (yytoken) {
	case TYPE:
	case TYPEIDEN:
		tp = typename();
		break;
	default:
		tp = typeof(unary());
		break;
	}
	expect(')');
	return tp;
}

static Node *cast(void);

static Node *
unary(void)
{
	Node *(*fun)(char, Node *);
	char op;
	Type *tp;

	switch (yytoken) {
	case SIZEOF:
		next();
		tp = (yytoken == '(') ? sizeexp() : typeof(unary());
		if (!tp->defined)
			errorp("sizeof applied to an incomplete type");
		return sizeofnode(tp);
	case INC:
	case DEC:
		op = (yytoken == INC) ? OA_ADD : OA_SUB;
		next();
		return incdec(unary(), op);
	case '!': op = 0;     fun = negation;     break;
	case '+': op = OADD;  fun = numericaluop; break;
	case '-': op = ONEG;  fun = numericaluop; break;
	case '~': op = OCPL;  fun = integeruop;   break;
	case '&': op = OADDR; fun = address;      break;
	case '*': op = OPTR;  fun = content;      break;
	default:  return postfix(NULL);
	}

	next();
	return (*fun)(op, cast());
}

static Node *
cast(void)
{
	Node *lp, *rp;
	Type *tp;
	static int nested;

	if (!accept('('))
		return unary();

	switch (yytoken) {
	case TQUALIFIER:
	case TYPE:
		tp = typename();
		switch (tp->op) {
		case ARY:
			error("cast specify an array type");
		case FTN:
			error("cast specify a function type");
		default:
			expect(')');
			if ((lp = eval(cast())) == NULL)
				unexpected();
			if ((rp = convert(lp,  tp, 1)) == NULL)
				error("bad type convertion requested");
			rp->lvalue = lp->lvalue;
		}
		break;
	default:
		if (nested == NR_SUBEXPR)
			error("too expressions nested by parentheses");
		++nested;
		rp = expr();
		--nested;
		expect(')');
		rp = postfix(rp);
		break;
	}

	return rp;
}

static Node *
mul(void)
{
	Node *np, *(*fun)(char, Node *, Node *);
	char op;

	np = cast();
	for (;;) {
		switch (yytoken) {
		case '*': op = OMUL; fun = arithmetic; break;
		case '/': op = ODIV; fun = arithmetic; break;
		case '%': op = OMOD; fun = integerop;  break;
		default: return np;
		}
		next();
		np = (*fun)(op, np, cast());
	}
}

static Node *
add(void)
{
	char op;
	Node *np;

	np = mul();
	for (;;) {
		switch (yytoken) {
		case '+': op = OADD; break;
		case '-': op = OSUB; break;
		default:  return np;
		}
		next();
		np = arithmetic(op, np, mul());
	}
}

static Node *
shift(void)
{
	char op;
	Node *np;

	np = add();
	for (;;) {
		switch (yytoken) {
		case SHL: op = OSHL; break;
		case SHR: op = OSHR; break;
		default:  return np;
		}
		next();
		np = integerop(op, np, add());
	}
}

static Node *
relational(void)
{
	char op;
	Node *np;

	np = shift();
	for (;;) {
		switch (yytoken) {
		case '<': op = OLT; break;
		case '>': op = OGT; break;
		case GE:  op = OGE; break;
		case LE:  op = OLE; break;
		default:  return np;
		}
		next();
		np = compare(op, np, shift());
	}
}

static Node *
eq(void)
{
	char op;
	Node *np;

	np = relational();
	for (;;) {
		switch (yytoken) {
		case EQ: op = OEQ; break;
		case NE: op = ONE; break;
		default: return np;
		}
		next();
		np = compare(op, np, relational());
	}
}

static Node *
bit_and(void)
{
	Node *np;

	np = eq();
	while (accept('&'))
		np = integerop(OBAND, np, eq());
	return np;
}

static Node *
bit_xor(void)
{
	Node *np;

	np = bit_and();
	while (accept('^'))
		np = integerop(OBXOR,  np, bit_and());
	return np;
}

static Node *
bit_or(void)
{
	Node *np;

	np = bit_xor();
	while (accept('|'))
		np = integerop(OBOR, np, bit_xor());
	return np;
}

static Node *
and(void)
{
	Node *np;

	np = bit_or();
	while (accept(AND))
		np = logic(OAND, np, bit_or());
	return np;
}

static Node *
or(void)
{
	Node *np;

	np = and();
	while (accept(OR))
		np = logic(OOR, np, and());
	return np;
}

static Node *
ternary(void)
{
	Node *cond;

	cond = or();
	while (accept('?')) {
		Node *ifyes, *ifno, *np;

		cond = exp2cond(cond, 0);
		ifyes = promote(expr());
		expect(':');
		ifno = promote(ternary());
		typeconv(&ifyes, &ifno);
		if (cond->constant) {
			TINT i = cond->sym->u.i;

			freetree(cond);
			if (i == 0) {
				freetree(ifyes);
				return ifno;
			} else {
				freetree(ifno);
				return ifyes;
			}
		}
		np = node(OCOLON, ifyes->type, ifyes, ifno);
		cond = node(OASK, np->type, cond, np);
	}
	return cond;
}

static Node *
assign(void)
{
	Node *np, *(*fun)(char , Node *, Node *);
	char op;

	np = ternary();
	for (;;) {
		switch (yytoken) {
		case '=':    op = OASSIGN; fun = assignop;   break;
		case MUL_EQ: op = OA_MUL;  fun = arithmetic; break;
		case DIV_EQ: op = OA_DIV;  fun = arithmetic; break;
		case MOD_EQ: op = OA_MOD;  fun = integerop;  break;
		case ADD_EQ: op = OA_ADD;  fun = arithmetic; break;
		case SUB_EQ: op = OA_SUB;  fun = arithmetic; break;
		case SHL_EQ: op = OA_SHL;  fun = integerop;  break;
		case SHR_EQ: op = OA_SHR;  fun = integerop;  break;
		case AND_EQ: op = OA_AND;  fun = integerop;  break;
		case XOR_EQ: op = OA_XOR;  fun = integerop;  break;
		case OR_EQ:  op = OA_OR;   fun = integerop;  break;
		default: return np;
		}
		chklvalue(np, np->type);
		next();
		np = (fun)(op, np, assign());
	}
}

Node *
constexpr(void)
{
	Node *np;

	np = ternary();
	if (!np->constant) {
		freetree(np);
		return NULL;
	}
	return np;
}

Node *
iconstexpr(void)
{
	Node *np;

	if ((np = constexpr()) == NULL)
		return NULL;

	if (np->type->op != INT) {
		freetree(np);
		return NULL;
	}

	return convert(np, inttype, 0);
}

Node *
expr(void)
{
	Node *lp, *rp;

	lp = assign();
	while (accept(',')) {
		rp = assign();
		lp = node(OCOMMA, rp->type, lp, rp);
	}

	return lp;
}

Node *
condition(void)
{
	Node *np;

	expect('(');
	np = exp2cond(expr(), 0);
	if (np->constant)
		warn("conditional expression is constant");
	expect(')');

	return np;
}
