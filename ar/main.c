static char sccsid[] = "@(#) ./ar/main.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <stat.h>

#include "../inc/ar.h"
#include "../inc/arg.h"
#include "../inc/scc.h"

char *argv0;

static int bflag, iflag, vflag, cflag, lflag, uflag, aflag;
static int done;
static char *afile, *posname;

static void
cleanup(void)
{
	if (!done)
		remove(afile);
}

static void
usage(void)
{
	fputs("ar [-drqtpmx][posname] [-vuaibcl] [posname] afile name ...\n", stderr);
	exit(1);
}

FILE *
openar(char *afile)
{
	FILE *fp;
	char magic[SARMAG+1];
	struct stat st;

	if ((fp = fopen(afile,"rb")) == NULL) {
		if (!cflag)
			fprintf(stderr, "ar: creating %s\n", afile);
		if ((fopen(afile, "w+b")) == NULL)
			goto file_error;
		fputs(ARMAG, fp);
		fflush(fp);
	} else {
		if (fgets(magic, sizeof(magic), fp) == NULL)
			goto file_error;
		if (!strcmp(magic, ARMAG)) {
			fprintf(stderr,
			        "ar:%s:invalid magic number '%s'\n",
			        afile,
			        magic);
			exit(1);
		}
	}
	if (ferror(fp))
		goto file_error;
	return fp;

file_error:
	perror("ar:opening archive");
	exit(1);
}

static void
archieve(char *fname, FILE *to)
{
	int c;
	size_t n;
	FILE *from;
	char mtime[13];
	struct stat st;

	if (strlen(fname) > 16)
		fprintf(stderr, "ar:%s: too long name\n", fname);
	if (stat(fname, &st) < 0) {
		fprintf(stderr, "ar:error getting '%s' attributes\n", fname);
		exit(1);
	}
	if ((from = fopen(fname, "rb")) == NULL) {
		fprintf(stderr,
		        "ar:opening member '%s':%s\n",
		        fname,
		        strerror(errno));
		exit(1);
	}
	strftime(mtime, sizeof(mtime), "%s", gmtime(&st.st_mtime));
	fprintf(to,
	        "%-16.16s%-12s%-6u%-6u%-8o%-10llu`\n",
	        fname,
	        mtime,
	        st.st_uid,
	        st.st_gid,
	        st.st_mode,
	        st.st_size);
	for (n = 0; (c = getc(from)) != EOF; n++)
		putc(c, to);
	if (n & 1)
		putc('\n', to);
	if (ferror(from)) {
		fprintf(stderr,
		        "ar:reading input '%s':%s\n",
		        fname, strerror(errno));
		exit(1);
	}
	fclose(from);
}

static void
append(FILE *fp, char *list[])
{
	char *fname;

	if (fseek(fp, 0, SEEK_END) == EOF) {
		perror("ar:seeking archive");
		exit(1);
	}
	while ((fname = *list++) != NULL) {
		if (vflag)
			printf("a - %s\n", fname);
		archieve(fname, fp);
	}
}

int
main(int argc, char *argv[])
{
	int key, nkey = 0, pos = 0;
	FILE *fp;

	atexit(cleanup);
	ARGBEGIN {
	case 'd':
		nkey++;
		key = 'd';
		break;
	case 'r':
		key = 'r';
		break;
	case 'q':
		nkey++;
		key = 'q';
		break;
	case 't':
		nkey++;
		key = 't';
		break;
	case 'p':
		nkey++;
		key = 'p';
		break;
	case 'm':
		nkey++;
		key = 'm';
		break;
	case 'x':
		nkey++;
		key = 'x';
		break;

	case 'a':
		aflag = 1;
		pos++;
		posname = EARGF(usage());
		break;
	case 'b':
		bflag = 1;
		pos++;
		posname = EARGF(usage());
		break;
	case 'i':
		iflag = 1;
		pos++;
		posname = EARGF(usage());
		break;
	case 'v':
		vflag = 1;
		break;
	case 'c':
		cflag = 1;
		break;
	case 'l':
		lflag = 1;
		break;
	case 'u':
		uflag = 1;
		break;
	default:
		usage();
	} ARGEND

	if (nkey == 0 || nkey > 1 || pos > 1 || argc == 0)
		usage();
	afile = *argv++;
	fp = openar(afile);

	switch (key) {
	case 'q':
		append(fp, argv);
		break;
	case 'd':
	case 'r':
	case 't':
	case 'p':
	case 'm':
	case 'x':
		/* TODO */
		;
	}

	if (ferror(fp)) {
		perror("ar:error reading archive");
		exit(1);
	}


	/* TODO: check status of stdout */
	done = 1;

	return 0;

}
