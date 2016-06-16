/* See LICENSE file for copyright and license details. */
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../inc/arg.h"
#include "../inc/cc.h"
#include "arch.h"
#include "cc1.h"

char *argv0;

int warnings;
jmp_buf recover;

static char *base, *output;
int onlycpp;

extern int failure;

static void
clean(void)
{
	if (failure && output)
		remove(output);
}

static void
usage(void)
{
	die(!strcmp(base, "cpp") ?
	    "usage: cpp [-wd] [-D def[=val]]... [-U def]... [-I dir]... "
	    "[input]" :
	    "usage: cc1 [-Ewd] [-D def[=val]]... [-U def]... [-I dir]... "
	    "[-o output] [input]");
}

int
main(int argc, char *argv[])
{
	char *base;
	static char *uvec[NR_USWITCHES], **umacro = uvec;

	atexit(clean);
	icpp();

	/* if run as cpp, only run the preprocessor */
	if ((base = strrchr(argv0, '/')))
		++base;
	else
		base = argv0;

	ARGBEGIN {
	case 'D':
		defmacro(EARGF(usage()));
		break;
	case 'E':
		onlycpp = 1;
		break;
	case 'I':
		incdir(EARGF(usage()));
		break;
	case 'U':
		if (umacro == &uvec[NR_USWITCHES])
			die("too many -U switches");
		*umacro++ = EARGF(usage());
		break;
	case 'd':
		DBGON();
		break;
	case 'o':
		output = EARGF(usage());
		break;
	case 'w':
		warnings = 1;
		break;
	default:
		usage();
	} ARGEND

	if (argc > 1)
		usage();

	if (output && !freopen(output, "w", stdout))
		die("error opening output: %s", strerror(errno));

	if (!strcmp(base, "cpp"))
		onlycpp = 1;

	for (umacro = uvec; *umacro; umacro++)
		undefmacro(*umacro);

	ilex(*argv);
	if (onlycpp) {
		outcpp();
	} else {
		next();

		while (yytoken != EOFTOK)
			decl();
	}

	return failure;
}
