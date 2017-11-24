static char sccsid[] = "@(#) ./lib/scc/war.c";

#include <assert.h>
#include <stdio.h>

#include "../../inc/ar.h"

int
wrarhdr(FILE *fp, struct arhdr *hdr)
{
	int len;

	len = fprintf(fp,
	              "%-16s%-12llu%-6u%-6u%-8o%-10llu`\n",
	              hdr->name,
	              hdr->time,
	              hdr->uid, hdr->gid,
	              hdr->mode,
	              hdr->size);
	assert(len== ARHDR_SIZ);

	return (feof(fp)) ? EOF : len;
}

int
wrarfile(FILE *fo, struct arhdr *hdr)
{
	FILE *fi;
	int c;

	if ((fi = fopen(hdr->name, "rb")) == NULL)
		return -1;
	while ((c = getc(fi)) != EOF)
		putc(c, fo);
	if (hdr->size & 1)
		putc('\n', fo);
	return (fclose(fi) == EOF) ? -1 : 0;
}
