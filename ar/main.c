static char sccsid[] = "@(#) ./ar/main.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stat.h>

int
main(int argc, char *argv[])
{
	int c;
	size_t n;
	FILE *fp, *arfile;
	char *fname, *arname = "lib.a";
	struct stat st;

	if ((arfile = fopen(arname, "wb")) == NULL) {
		perror("ar:error opening library file");
		exit(1);
	}

	fputs("!<arch>\n", arfile);
	while ((fname = *++argv) != NULL) {
		if ((n = strlen(fname)) > 16) {
			fprintf(stderr, "ar:too long file name '%s'\n", fname);
			exit(3);
		}
		if (stat(fname, &st) < 0) {
			fprintf(stderr,
			        "ar:error opening object file '%s':%s\n",
			        fname, strerror(errno));
			exit(2);
		}
		fprintf(arfile,
		        "%-16s%-12llu%-6u%-6u%-8o%-10llu`\n",
		        fname,
		        (unsigned long long) st.st_atime,
		        (int) st.st_uid, (int) st.st_gid,
		        (int) st.st_mode,
		        (unsigned long long) st.st_size);
		if ((fp = fopen(fname, "rb")) == NULL) {
			fprintf(stderr,
			        "ar: error opening file '%s':%s\n",
			        fname, strerror(errno));
			exit(3);
		}
		while ((c = getc(fp)) != EOF)
			putc(c, arfile);
		if (st.st_size & 1)
			putc('\n', arfile);
		if (fclose(fp)) {
			fprintf(stderr,
			        "ar:error reading from input file '%s':%s\n",
			        fname, strerror(errno));
			exit(4);
		}
	}

	if (fclose(arfile)) {
		fprintf(stderr,
		        "ar:error writing to output file '%s':%s\n",
		        arname, strerror(errno));
	}

	return 0;
}
