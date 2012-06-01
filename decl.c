#include <alloca.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

#include "cc.h"
#include "tokens.h"
#include "symbol.h"
#include "types.h"


char parser_out_home;

static unsigned char symhash;
static char symname[TOKSIZ_MAX + 1];


#include <stdio.h>	/* TODO: remove this */

static void declarator(void);


static void dirdcl(void)
{
	puts("dirdecl");
	if (accept('(')) {
		declarator();
		expect(')');
	} else if (accept(IDENTIFIER)) {
		strcpy(symname, yytext);
		symhash = yyhash;
	} else {
		error("expected '(' or identifier before of '%s'", yytext);
	}

	for (;;) {
		if (accept('(')) {
			next();
			pushtype(FTN);
			if (accept(')'))
				; /* TODO: k&r function */
			else
				/* TODO: prototyped function */;
			continue;
		} else if (accept('[')) {
			pushtype(ARY);
			if (accept(']'))
				; /* TODO: automatic size array */
			else
				/* TODO: specify size of array */;
			continue;
		} else {
			printf("leaving dirdcl %c\n", yytoken);
			return;
		}
	}
}





/*
 *
 */

static struct type *types[][2] = {{T_VOID, NULL},
				  {T_SCHAR, T_UCHAR},
				  {T_SHORT, T_USHORT},
				  {T_INT, T_UINT},
				  {T_LONG, T_ULONG},
				  {T_LLONG, T_ULLONG},
				  {T_FLOAT, NULL},
				  {T_DOUBLE, NULL},
				  {T_LDOUBLE, NULL}};

#define F_VOID    0
#define F_CHAR    1
#define F_SHORT   2
#define F_INT     3
#define F_LONG    4
#define F_LLONG   5
#define F_FLOAT   6
#define F_DOUBLE  7
#define F_LDOUBLE 8

static struct type *specifier(void)
{
	static char sign, sclass, tqlf, nt;
	struct type *t;

repeat:
	t = NULL;
	tqlf = sign = sclass = 0;
	for (;; next()) {
		switch (yytoken) {
		case TYPEDEF:case EXTERN:case STATIC:case AUTO:case REGISTER:
			if (sclass != 0)
				error("Two or more storage specifier");
			sclass = yytoken;
			continue;
		case CONST: case VOLATILE: case RESTRICTED:
			/* TODO */
			continue;
		case VOID:   nt = F_VOID;   goto check_type;
		case CHAR:   nt = F_CHAR;   goto check_type;
		case SHORT:  nt = F_SHORT;  goto check_type;
		case INT:    nt = F_INT;    goto check_type;
		case FLOAT:  nt = F_FLOAT;  goto check_type;
		case DOUBLE: nt = F_DOUBLE; goto check_type;
		case LONG:   nt = F_LONG;   goto check_type;
		case SIGNED: case UNSIGNED:
			if (sign != 0) {
				error((sign != yytoken) ?
				      "signed and unsigned in declaration" :
				      "duplicated %s", yytext);
			}
			sign = yytoken;
			if (t == NULL)
				continue;     /* we don't have type now */
			goto check_type;
		case STRUCT:	/* TODO */
		case UNION:	/* TODO */
		case ENUM:	/* TODO */
		case ';':
			if (t != NULL) {
				warning("useless type name in empty "
					"declaration");
			}
			goto repeat;
		case IDENTIFIER:
			/* TODO: deal with user types */
			if (t == NULL) {
				warning_error(user_opt.implicit_int,
					      "type defaults to 'int' "
					      "in declaration of", yytext);
				return T_INT;
			}
		default:
			return t;
		}
	check_type:
		if (nt == F_LONG) {
			if (t == NULL ||
			    t == T_INT || t == T_UINT || t == T_LONG) {
				/* nothing */;
			} else if (t == T_LONG || t == T_ULONG) {
				nt = F_LLONG;
			} else if (t == T_DOUBLE) {
				nt = F_LDOUBLE;
			} else if (t == T_LLONG || t == T_ULLONG) {
				error("'long long long' is too long");
			} else if (t == T_LDOUBLE) {
				error("'long long double' is too long");
			} else {
				goto two_or_more_btype;
			}
		} else if (t != NULL) {
			goto two_or_more_btype;
		} if (nt == F_VOID && sign != 0) {
			goto incorrect_sign;
		} if (nt == F_CHAR && sign == 0) {
			t = T_UCHAR;	    /* char by default is unsigned */
		} else if (!(t = types[nt][sign == UNSIGNED])) {
			goto incorrect_sign;
		}
	}
two_or_more_btype:
	error("two or more basic types");
incorrect_sign:
	error("sign specifier applied to incorrect type");
}


#undef F_VOID
#undef F_CHAR
#undef F_SHORT
#undef F_INT
#undef F_LONG
#undef F_LLong
#undef F_FLOAT
#undef F_DOUBLE
#undef F_LDOUBLE


static void declarator(void)
{
	unsigned char qlf[PTRLEVEL_MAX], *bp, *lim;

	puts("declarator");
	lim = qlf + PTRLEVEL_MAX;
	for (bp = qlf; yytoken == '*' && bp != lim; ++bp) {
		*bp = 0;
	repeat_qlf:
		switch (next()) {
		case CONST:
			if (!(*bp ^= 1))
				goto duplicated;
			goto repeat_qlf;
		case RESTRICTED:
			if (!(*bp ^= 2))
				goto duplicated;
			goto repeat_qlf;
		case VOLATILE:
			if (!(*bp ^= 4))
				goto duplicated;
			goto repeat_qlf;
		default:
			break;
		}
	}

	if (bp == lim)
		error("Too much indirection levels");

	dirdcl();

	while (bp-- != qlf) {
		if (*bp & 1)  pushtype(CONST);
		if (*bp & 2)  pushtype(RESTRICTED);
		if (*bp & 4)  pushtype(VOLATILE);
		pushtype(PTR);
	}

	printf("leaving dcl %c\n", yytoken);
	return;

duplicated:
	error("duplicated '%s'", yytext);
}

void decl(void)
{
	struct type *t, *spec;

	spec = specifier();
	do {
		declarator();
		t = decl_type(spec);
	} while (accept(','));
	puts("leaving declaration");
}

void stmt(void)
{
	for (;;) {
		decl();
		expect(';');
	}

}

int main(int argc, char *argv[])
{
	init_lex();

	open_file(NULL);
	next();
	stmt();

	return 0;
}
