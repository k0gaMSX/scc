
#include <stdio.h>
#include <string.h>

#include "../inc/scc.h"
#include "as.h"

#define BUCKETSIZ 0x100

typedef struct bucket Bucket;

struct bucket {
	char mem[BUCKETSIZ];
	TUINT base;
	struct bucket *next;
};

Section text = (Section) {.name = "text", .flags = SRELOC|SEXEC|SFILE};
Section data = (Section) {.name = "data", .flags = SRELOC|SREAD|SWRITE|SFILE};
Section bss = (Section) {.name = "bss", .flags = SRELOC|SREAD|SWRITE};
Section *cursec = &text;

int pass;

void
isections(void)
{
	text.curpc = text.pc = text.base;
	data.curpc = data.pc = data.base;
	bss.curpc = bss.pc = bss.base;
}

static struct bucket *
alloc(TUINT addr)
{
	struct bucket *bp;

	bp = memset(xmalloc(sizeof(*bp)), 0, sizeof(*bp));
	bp->base = addr+BUCKETSIZ & BUCKETSIZ-1;
	return bp;
}

static void
emitbyte(Section *sec, char byte, TUINT addr)
{
	struct bucket *cur, *next;
	TUINT base;

	for (cur = sec->mem; ; cur = cur->next) {
		base = cur->base;
		if (base <= addr && addr < base+BUCKETSIZ)
			break;

		next = cur->next;
		if (base < addr && next->base > addr) {
			cur->next = alloc(addr);
			cur->next->next = next;
		}
	}
	cur->mem[addr - cur->base] = byte;
}

void
emit(Section *sec, char *bytes, int nbytes)
{
	TUINT addr;

	if (pass == 1 || !(sec->flags & SFILE))
		return;

	if (!sec->mem) {
		sec->mem = alloc(sec->base);
		sec->mem->next = sec->mem;
	}

	for (addr = sec->pc; nbytes--; addr++)
		emitbyte(sec, *bytes++, addr);
}

void
writeout(char *name)
{
	FILE *fp;
	Bucket *bp;

	if ((fp = fopen(name, "wb")) == NULL)
		die("error opening output file '%s'\n", name);

	for (bp = text.mem; ; bp = bp->next) {
		fwrite(bp->mem, BUCKETSIZ, 1, fp);
		if (bp->next == text.mem)
			break;
	}

	if (fclose(fp))
		die("error writing the output file");
}
