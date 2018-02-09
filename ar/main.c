static char sccsid[] = "@(#) ./ar/main.c";

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stat.h>

#include "../inc/ar.h"
#include "../inc/arg.h"
#include "../inc/scc.h"

char *argv0;

int dflag, rflag, qflag, tflag, pflag, mflag, bflag,
    iflag, xflag, vflag, cflag, lflag, uflag, aflag;
    
char *posname;

static void
usage(void)
{
	fputs("ar [-drqtpmx][posname] [-vuaibcl] [posname] afile name ...\n", stderr);
	exit(1);
}

int
main(int argc, char *argv[])
{
	int key = 0, pos = 0;

	ARGBEGIN {
	case 'd':
		dflag = 1;
		key++;
		break;
	case 'r':
		rflag = 1;
		key++;
		break;
	case 'q':
		qflag = 1;
		key++;
		break;
	case 't':
		tflag = 1;
		key++;
		break;
	case 'p':
		pflag = 1;
		key++;
		break;
	case 'm':
		mflag = 1;
		key++;
		break;
	case 'x':
		xflag = 1;
		key++;
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

	if (key == 0 || key > 1 || pos > 1)
		usage();

	return 0;
}
