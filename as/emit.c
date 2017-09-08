
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

static Section bss = {
	.name = "bss",
	.flags = SRELOC|SREAD|SWRITE
};

static Section data = {
	.name = "data",
	.next = &bss,
	.flags = SRELOC|SREAD|SWRITE|SFILE
};

static Section text = {
	.name = "text",
	.next = &data,
	.flags = SRELOC|SEXEC|SFILE
};

Section *cursec = &text, *headp = &text;

int pass;

static void
isect(Section *sec)
{
	TUINT siz;

	sec->curpc = sec->pc = sec->base;
	if (pass == 1 || !(sec->flags & SFILE))
		return;

	siz = sec->max - sec->base;
	if (siz > SIZE_MAX)
		die("out of memory");
	sec->mem = xmalloc(sec->max - sec->base);
}

Section *
addsect(char *name, TUINT base, int flags)
{
	Section *sec;

}

Section *
section(char *name)
{
	Section *sec;

	for (sec = headp; sec; sec = sec->next) {
		if (!strcmp(sec->name, name))
			break;
	}
	if (!sec) {
		sec = xmalloc(sizeof(*sec));
		sec->name = xstrdup(name);
		sec->base = sec->max = sec->pc = sec->curpc = 0;
		sec->next = headp;
		sec->flags = SRELOC|SREAD|SWRITE|SFILE;
	}
	cursec = sec;
}

void
isections(void)
{
	Section *sec;

	for (sec = headp; sec; sec = sec->next)
		isect(sec);
}

void
emit(Section *sec, char *bytes, int nbytes)
{
	TUINT addr;

	if (!sec->mem)
		return;

	for (addr = sec->pc - sec->base; nbytes--; addr++)
		sec->mem[addr] = *bytes++;
}

void
writeout(char *name)
{
	FILE *fp;
	Section *secp;

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	for (secp = headp; secp; secp = secp->next) {
		if (!secp->mem)
			continue;
		fwrite(secp->mem, secp->max - secp->base, 1, fp);
	}

	if (fclose(fp))
		die("error writing the output file");
}
