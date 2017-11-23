static char sccsid[] = "@(#) ./lib/scc/wmyro.c";

#include <stdio.h>

#include "../../inc/scc.h"
#include "../../inc/myro.h"

size_t
writehdr(FILE *fp, struct myrohdr *hdr)
{
	unsigned char buf[sizeof(*hdr)];
	size_t len;

	len = lpack(buf, "lqqqqq",
	            hdr->format,
	            hdr->entry,
	            hdr->strsize,
	            hdr->secsize,
	            hdr->symsize,
	            hdr->relsize);
	fwrite(buf, len, 1, fp);

	return len;
}

size_t
writesec(FILE *fp, struct myrosect *sect)
{
	unsigned char buf[sizeof(*sect)];
	size_t len;

	len = lpack(buf, "lsccqq",
	            sect->name,
	            sect->flags,
	            sect->fill,
	            sect->aligment,
	            sect->offset,
	            sect->len);
	fwrite(buf, len, 1, fp);

	return len;
}

size_t
writesym(FILE *fp, struct myrosym *sym)
{
	unsigned char buf[sizeof(*sym)];
	size_t len;

	len = lpack(buf, "llccqq",
	            sym->name,
	            sym->type,
	            sym->section,
	            sym->flags,
	            sym->offset,
	            sym->len);
	fwrite(buf, len, 1, fp);

	return len;
}

size_t
writerel(FILE *fp, struct myrorel *rel)
{
	unsigned char buf[sizeof(*rel)];
	size_t len;

	len = lpack(buf, "lccccq",
	            rel->id,
	            rel->flags,
	            rel->size,
	            rel->nbits,
	            rel->shift,
	            rel->offset);
	fwrite(buf, len, 1, fp);

	return len;
}
