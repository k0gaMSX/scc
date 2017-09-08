
#include <stdio.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

Section text = (Section) {.name = "text", .flags = SRELOC|SEXEC|SFILE};
Section data = (Section) {.name = "data", .flags = SRELOC|SREAD|SWRITE|SFILE};
Section bss = (Section) {.name = "bss", .flags = SRELOC|SREAD|SWRITE};
Section *cursec = &text;

int pass;

static void
isec(Section *sec)
{
	sec->curpc = sec->pc = sec->base;
	if (sec->max > 0)
		sec->mem = xmalloc(sec->max - sec->base);
}

void
isections(void)
{
	isec(&text);
	isec(&data);
	isec(&bss);
}

void
emit(Section *sec, char *bytes, int nbytes)
{
	TUINT addr;

	if (pass == 1 || !(sec->flags & SFILE))
		return;

	for (addr = sec->pc - sec->base; nbytes--; addr++)
		sec->mem[addr] = *bytes++;
}

void
writeout(char *name)
{
	FILE *fp;

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	fwrite(text.mem, text.max - text.base, 1, fp);

	if (fclose(fp))
		die("error writing the output file");
}
