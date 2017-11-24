static char sccsid[] = "@(#) ./lib/scc/rar.c";

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../../inc/ar.h"

int
rdarhdr(FILE *fp, struct arhdr *hdr)
{
	char buf[ARHDR_SIZ+1];
	size_t len;
	int n;

	if (!fgets(buf, sizeof(buf), fp))
		return EOF;
	if ((len = strlen(buf)) != ARHDR_SIZ ||
	    buf[len-2] != '`' ||
	    buf[len-1] != '\n') {
		return EOF;
	}

	n = sscanf(buf, "%16s-%llu-%u-%u-%o-%llu",
	           &hdr->name,
	           &hdr->time,
	           &hdr->uid, &hdr->gid,
	           &hdr->mode,
	           &hdr->size);
	if (n != 6)
		return EOF;
	return (feof(fp)) ? EOF : 0;
}
