
#include <stdlib.h>
#include <string.h>

#include "../inc/cc.h"

#include "arch.h"
#include "cc2.h"

#define NNODES   32

Symbol *curfun;
Type rtype;

struct arena {
	Node *mem;
	struct arena *next;
};

static struct arena *arena;
static Node *freep, *stmtp;
static int inhome;

Node *
newnode(void)
{
	struct arena *ap;
	Node *np;

	if (!freep) {
		ap = xmalloc(sizeof(*ap));
		ap->mem = xcalloc(NNODES, sizeof(Node));
		ap->next = arena;
		arena = ap;
		for (np = ap->mem; np < &ap->mem[NNODES-1]; ++np)
			np->left = np+1;
		np->left = NULL;
		freep = np;
	}

	np = freep;
	freep = np->left;

	return memset(np, 0, sizeof(*np));
}

Node *
addstmt(Node *np)
{
	if (!curfun->u.stmt)
		curfun->u.stmt = np;
	else
		stmtp->next = np;
	np->next = NULL;
	np->prev = stmtp;
	stmtp = np;

	return np;
}

void
delnode(Node *np)
{
	np->left = freep;
	freep = np;
}

void
deltree(Node *np)
{
	if (!np)
		return;
	deltree(np->left);
	deltree(np->right);
	delnode(np);
}

void
cleannodes(void)
{
	struct arena *ap, *next;

	for (ap = arena; ap; ap = next) {
		next = ap->next;
		free(ap->mem);
		free(ap);
	}
	arena = NULL;
	freep = NULL;
	stmtp = NULL;
}

void
apply(Node *(*fun)(Node *))
{
	Node *np;

	if (!curfun)
		return;

	for (np = curfun->u.stmt; np; np = np->next)
		(*fun)(np);
}
