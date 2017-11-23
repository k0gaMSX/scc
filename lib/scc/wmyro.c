static char sccsid[] = "@(#) ./lib/scc/wmyro.c";

#include <stdio.h>

#include "../../inc/scc.h"
#include "../../inc/myro.h"

int
writehdr(FILE *fp, struct myrohdr *hdr)
{
	unsigned char buf[MYROHDR_SIZ];
	int len;

	len = lpack(buf, "lqqqqq",
	            hdr->format,
	            hdr->entry,
	            hdr->strsize,
	            hdr->secsize,
	            hdr->symsize,
	            hdr->relsize);
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}

int
writesec(FILE *fp, struct myrosect *sect)
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
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}

int
writesym(FILE *fp, struct myrosym *sym)
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
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}

int
writerel(FILE *fp, struct myrorel *rel)
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
	fwrite(buf, len, 1, fp);

	return (ferror(fp)) ? EOF : len;
}
