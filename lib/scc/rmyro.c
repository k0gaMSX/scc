static char sccsid[] = "@(#) ./lib/scc/rmyro.c";

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../inc/scc.h"
#include "../../inc/myro.h"

int
rdmyrohdr(FILE *fp, struct myrohdr *hdr)
{
	unsigned char buf[MYROHDR_SIZ];
	int len;

	fread(buf, sizeof(buf), 1, fp);
	if (ferror(fp))
		return EOF;
	len = lunpack(buf, "cccclqqqqq",
	              hdr->magic+0, hdr->magic+1,
	              hdr->magic+2, hdr->magic+3,
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
rdmyrosec(FILE *fp, struct myrosect *sect)
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
rdmyrosym(FILE *fp, struct myrosym *sym)
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
rdmyrorel(FILE *fp, struct myrorel *rel)
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
