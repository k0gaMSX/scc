
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/scc.h"
#include "../inc/myro.h"

char *strings;
size_t strsiz;

static char *
getstring(unsigned long off)
{
	size_t n;

	
	if (off < SIZE_MAX) {
		for (n = off; n < strsiz && strings[n]; ++n)
			;
		if (n != strsiz)
			return &strings[off];
	}
	fprintf(stderr, "objdump: wrong string offset %lu\n", off);
	return "";
}

static void
printhdr(struct myrohdr *hdr)
{
	printf("header:\n"
	       "\tmagic: %02x%02x%02x%02x \"%4.4s\"\n"
	       "\tformat: %lu (\"%s\")\n"
	       "\tentry: %llu\n"
	       "\tstring table size: %llu\n"
	       "\tsection table size: %llu\n"
	       "\tsymbol table size: %llu\n"
	       "\trelocation table size: %llu\n",
	       hdr->magic[0], hdr->magic[1],
	       hdr->magic[2], hdr->magic[3],
	       hdr->magic,
	       hdr->format, getstring(hdr->format),
	       hdr->entry,
	       hdr->strsize,
	       hdr->secsize,
	       hdr->symsize,
	       hdr->relsize);
}

int
main(int argc, char *argv[])
{
	FILE *fp;
	struct myrohdr hdr;

	while (*++argv) {
		free(strings);
		strings = NULL;

		puts(*argv);

		if ((fp = fopen(*argv, "rb")) == NULL)
			goto wrong_file;
		if (rdmyrohdr(fp, &hdr) < 0)
			goto wrong_file;
		if (hdr.strsize > SIZE_MAX)
			goto overflow;
		strsiz = hdr.strsize;

		if (strsiz > 0) {
			strings = xmalloc(strsiz);
			fread(strings, strsiz, 1, fp);
			if (feof(fp))
				goto wrong_file;
		}

		printhdr(&hdr);


		goto close_file;
		
wrong_file:
		fprintf(stderr,
			"objdump: %s: %s\n",
			*argv, strerror(errno));
		goto close_file;

overflow:
		fprintf(stderr,
			"objdump: %s: overflow in header\n",
			*argv, strerror(errno));

close_file:
		if (fp)
			fclose(fp);
	}

	return 0;
}
