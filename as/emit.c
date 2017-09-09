
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
	.flags = SRELOC|SFILE
};

Section *cursec = &text, *headp = &text;

int pass;

char *
pack(TUINT v, int n, int inc)
{
	static char buf[sizeof(TUINT)];
	int idx;

	idx = (inc < 0) ? n-1 : 0;
	while (n--) {
		buf[idx] = v;
		idx += inc;
		v >>= 8;
	}
	return buf;
}

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
emit(Section *sec, char *bytes, int n)
{
	if (sec->mem)
		memcpy(&sec->mem[sec->pc - sec->base], bytes, n);
	incpc(n);
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
