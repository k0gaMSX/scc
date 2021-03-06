static char sccsid[] = "@(#) ./lib/scc/wmyro.c";

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../inc/scc.h"
#include "../../inc/myro.h"

int
wrmyrohdr(FILE *fp, struct myrohdr *hdr)
{
	unsigned char buf[MYROHDR_SIZ];
	int len;

	len = lpack(buf, "cccclqqqqq",
	            hdr->magic[0], hdr->magic[1],
	            hdr->magic[2], hdr->magic[3],
	            hdr->format,
	            hdr->entry,
	            hdr->strsize,
	            hdr->secsize,
	            hdr->symsize,
	            hdr->relsize);
	assert(MYROHDR_SIZ == len);
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}

int
wrmyrosec(FILE *fp, struct myrosect *sect)
{
	unsigned char buf[MYROSECT_SIZ];
	int len;

	len = lpack(buf, "lsccqq",
	            sect->name,
	            sect->flags,
	            sect->fill,
	            sect->aligment,
	            sect->offset,
	            sect->len);
	assert(MYROSECT_SIZ == len);
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}

int
wrmyrosym(FILE *fp, struct myrosym *sym)
{
	unsigned char buf[MYROSYM_SIZ];
	int len;

	len = lpack(buf, "llccqq",
	            sym->name,
	            sym->type,
	            sym->section,
	            sym->flags,
	            sym->offset,
	            sym->len);
	assert(MYROSYM_SIZ == len);
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}

int
wrmyrorel(FILE *fp, struct myrorel *rel)
{
	unsigned char buf[MYROREL_SIZ];
	int len;

	len = lpack(buf, "lccccq",
	            rel->id,
	            rel->flags,
	            rel->size,
	            rel->nbits,
	            rel->shift,
	            rel->offset);
	assert(MYROREL_SIZ == len);
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}
