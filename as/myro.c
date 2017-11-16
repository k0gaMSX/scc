static char sccsid[] = "@(#) ./as/myro.c";

#include <stdio.h>

#include "../inc/scc.h"
#include "as.h"

void
writeout(char *name)
{
	FILE *fp;
	Section *sp;

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	for (sp = seclist; sp; sp = sp->next) {
		if (!sp->mem)
			continue;
		fwrite(sp->mem, sp->max - sp->base, 1, fp);
	}

	if (fclose(fp))
		die("error writing the output file");
}
