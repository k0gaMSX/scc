static char sccsid[] = "@(#) ./lib/scc/rmyro.c";

#include <assert.h>
#include <stdio.h>

#include "../../inc/scc.h"
#include "../../inc/myro.h"

int
readhdr(FILE *fp, struct myrohdr *hdr)
{
	unsigned char buf[MYROHDR_SIZ];
	int len;

	fread(buf, sizeof(buf), 1, fp);
	if (ferror(fp))
		return EOF;
	len = lunpack(buf, "lqqqqq",
	              &hdr->format,
	              &hdr->entry,
	              &hdr->strsize,
	              &hdr->secsize,
	              &hdr->symsize,
	              &hdr->relsize);
	assert(len == MYROHDR_SIZ);

	return len;
}

int
readsec(FILE *fp, struct myrosect *sect)
{
	unsigned char buf[MYROSECT_SIZ];
	int len;

	fread(buf, sizeof(buf), 1, fp);
	if (ferror(fp))
		return EOF;
	len = lunpack(buf, "lsccqq",
	              &sect->name,
	              &sect->flags,
	              &sect->fill,
	              &sect->aligment,
	              &sect->offset,
	              &sect->len);
	assert(len == MYROSECT_SIZ);

	return len;
}

int
readsym(FILE *fp, struct myrosym *sym)
{
	unsigned char buf[MYROSYM_SIZ];
	int len;

	fread(buf, sizeof(buf), 1, fp);
	if (ferror(fp))
		return EOF;
	len = lunpack(buf, "llccqq",
	              &sym->name,
	              &sym->type,
	              &sym->section,
	              &sym->flags,
	              &sym->offset,
	              &sym->len);
	assert(len == MYROSYM_SIZ);

	return len;
}

int
readrel(FILE *fp, struct myrorel *rel)
{
	unsigned char buf[MYROREL_SIZ];
	int len;

	fread(buf, sizeof(buf), 1, fp);
	if (ferror(fp))
		return EOF;
	len = lunpack(buf, "lccccq",
	              &rel->id,
	              &rel->flags,
	              &rel->size,
	              &rel->nbits,
	              &rel->shift,
	              &rel->offset);
	assert(len == MYROREL_SIZ);

	return len;
}
