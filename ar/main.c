static char sccsid[] = "@(#) ./ar/main.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stat.h>

#include "../inc/scc.h"
#include "../inc/ar.h"

int
main(int argc, char *argv[])
{
	int c;
	size_t n;
	FILE *fp, *arfile;
	char *fname, *arname = "lib.a";
	struct stat st;
	struct arhdr hdr;

	if ((arfile = fopen(arname, "wb")) == NULL) {
		perror("ar:error opening library file");
		exit(1);
	}

	fputs(ARMAGIC, arfile);
	while ((fname = *++argv) != NULL) {
		if ((n = strlen(fname)) > ARNAME_SIZ)
			die("ar: %s: too long file name", fname);
		if (stat(fname, &st) < 0)
			goto member_error;

		strcpy(hdr.name, fname);
		hdr.time = st.st_atime;
		hdr.uid = st.st_uid;
		hdr.gid = st.st_gid;
		hdr.mode = st.st_mode;
		hdr.size = st.st_mode;

		if (wrarhdr(arfile, &hdr) < 0)
			goto member_error;
		if (wrarfile(arfile, &hdr) < 0)
			goto member_error;
	}

	if (fclose(arfile)) {
		fprintf(stderr,
		        "ar:error writing to output file '%s':%s\n",
		        arname, strerror(errno));
	}

	return 0;

member_error:
	die("ar: %s: %s", fname, strerror(errno));
}
