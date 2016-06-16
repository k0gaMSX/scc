/* See LICENSE file for copyright and license details. */
#define _POSIX_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/arg.h"
#include "../../inc/cc.h"

enum {
	CC1,
	TEEIR,
	CC2,
	TEEQBE,
	QBE,
	TEEAS,
	AS,
	LD,
	STRIP,
	LAST_TOOL,
};

static struct tool {
	char   cmd[PATH_MAX];
	char **args;
	char   bin[16];
	char  *outfile;
	int    nparams, nargs, in, out, init, error;
	pid_t  pid;
} tools[] = {
	[CC1]    = { .bin = "cc1",   .cmd = PREFIX "/libexec/scc/", },
	[TEEIR]  = { .bin = "tee",   .cmd = "tee", },
	[CC2]    = { .bin = "cc2",   .cmd = PREFIX "/libexec/scc/", },
	[TEEQBE] = { .bin = "tee",   .cmd = "tee", },
	[QBE]    = { .bin = "qbe",   .cmd = "qbe", },
	[TEEAS]  = { .bin = "tee",   .cmd = "tee", },
	[AS]     = { .bin = "as",    .cmd = "as", },
	[LD]     = { .bin = "gcc",   .cmd = "gcc", }, /* TODO use ld */
	[STRIP]  = { .bin = "strip", .cmd = "strip", },
};

struct objects {
	char **f;
	int n;
};

char *argv0;
static char *arch, *outfile;
static struct objects objtmp, objout;
static int Eflag, Sflag, cflag, kflag, sflag;

static void
terminate(void)
{
	struct tool *t;
	int i, tool, failed = LAST_TOOL;

	for (tool = 0; tool < LAST_TOOL; ++tool) {
		t = &tools[tool];
		if (t->pid) {
			kill(t->pid, SIGTERM);
			if (t->error)
				failed = tool;
			if (tool >= failed && t->outfile)
				unlink(t->outfile);
		}
	}

	if (!kflag) {
		for (i = 0; i < objtmp.n; ++i)
			unlink(objtmp.f[i]);
	}
}

static char **
newitem(char **array, int num, char *item)
{
	char **ar = xrealloc(array, (num + 1) * sizeof(char **));

	ar[num] = item;

	return ar;
}

static void
addarg(int tool, char *arg)
{
	struct tool *t = &tools[tool];

	if (t->nargs < 1)
		t->nargs = 1;

	t->args = newitem(t->args, t->nargs++, arg);
}

static void
setargv0(int tool, char *arg)
{
	struct tool *t = &tools[tool];

	if (t->nargs > 0)
		t->args[0] = arg;
	else
		t->args = newitem(t->args, t->nargs++, arg);
}

static int
inittool(int tool)
{
	struct tool *t = &tools[tool];
	size_t binln;
	int n;

	if (t->init)
		return tool;

	switch (tool) {
	case CC1: /* FALLTHROUGH */
	case CC2:
		binln = strlen(t->bin);
		if (arch) {
			n = snprintf(t->bin + binln,
				     sizeof(t->bin) - binln,
				     "-%s", arch);
			if (n < 0 || n >= sizeof(t->bin))
				die("scc: target tool bin too long");
			binln = strlen(t->bin);
		}

		if (strlen(t->cmd) + binln + 1 > sizeof(t->cmd))
			die("scc: target tool path too long");
		strcat(t->cmd, t->bin);
		break;
	case LD:
		addarg(tool, "-o");
		t->outfile = outfile ? outfile : xstrdup("a.out");
		addarg(tool, t->outfile);
		break;
	case AS:
		addarg(tool, "-o");
		break;
	default:
		break;
	}

	setargv0(tool, t->bin);
	t->nparams = t->nargs;
	t->init = 1;

	return tool;
}

static char *
outfilename(char *path, char *ext)
{
	char *new, *name, *dot;
	size_t newsz, nameln;
	int n;

	if (!(name = strrchr(path, '/')))
		name = path;
	else
		++name;

	nameln = strlen(name);

	if (!(dot = strrchr(name, '.')))
		dot = &name[nameln];

	nameln = nameln - strlen(dot);
	newsz  = nameln + strlen(ext) + 1 + 1;

	new = xmalloc(newsz);

	n = snprintf(new, newsz, "%.*s.%s", nameln, name, ext);
	if (n < 0 || n >= newsz)
		die("scc: wrong output filename");

	return new;
}

static int
settool(int tool, char *infile, int nexttool)
{
	struct tool *t = &tools[tool];
	int i, fds[2];
	static int fdin = -1;

	switch (tool) {
	case TEEIR:
		t->outfile = outfilename(infile, "ir");
		addarg(tool, t->outfile);
		break;
	case TEEQBE:
		t->outfile = outfilename(infile, "qbe");
		addarg(tool, t->outfile);
		break;
	case TEEAS:
		t->outfile = outfilename(infile, "as");
		addarg(tool, t->outfile);
		break;
	case AS:
		t->outfile = outfile ? outfile : outfilename(infile, "o");
		addarg(tool, t->outfile);
		break;
	case LD:
		for (i = 0; i < objtmp.n; ++i)
			addarg(tool, xstrdup(objtmp.f[i]));
		for (i = 0; i < objout.n; ++i)
			addarg(tool, xstrdup(objout.f[i]));
		break;
	case STRIP:
		if (cflag || kflag) {
			for (i = 0; i < objout.n; ++i)
				addarg(tool, xstrdup(objout.f[i]));
		}
		if (!cflag && tools[LD].outfile)
			addarg(tool, tools[LD].outfile);
		break;
	default:
		break;
	}

	if (fdin > -1) {
		t->in = fdin;
		fdin = -1;
	} else if (infile) {
		addarg(tool, xstrdup(infile));
	}

	if (nexttool < LAST_TOOL) {
		if (pipe(fds))
			die("scc: pipe: %s", strerror(errno));
		t->out = fds[1];
		fdin = fds[0];
	}

	addarg(tool, NULL);

	return tool;
}

static void
spawn(int tool)
{
	struct tool *t = &tools[tool];

	switch (t->pid = fork()) {
	case -1:
		die("scc: %s: %s", t->bin, strerror(errno));
	case 0:
		if (t->out > -1)
			dup2(t->out, 1);
		if (t->in > -1)
			dup2(t->in, 0);
		execvp(t->cmd, t->args);
		fprintf(stderr, "scc: execvp %s: %s\n",
		        t->cmd, strerror(errno));
		_exit(1);
	default:
		if (t->in > -1)
			close(t->in);
		if (t->out > -1)
			close(t->out);
		break;
	}
}

static int
toolfor(char *file)
{
	char *dot = strrchr(file, '.');

	if (dot) {
		if (!strcmp(dot, ".c"))
			return CC1;
		if (!strcmp(dot, ".ir"))
			return CC2;
		if (!strcmp(dot, ".qbe"))
			return QBE;
		if (!strcmp(dot, ".as"))
			return AS;
		if (!strcmp(dot, ".o"))
			return LD;
	}

	die("scc: do not recognize filetype of %s", file);
}

static void
validatetools(void)
{
	struct tool *t;
	int i, tool, st;
	for (tool = 0; tool < LAST_TOOL; ++tool) {
		t = &tools[tool];
		if (t->pid) {
			if (waitpid(t->pid, &st, 0) < 0 ||
			    !WIFEXITED(st) || WEXITSTATUS(st) != 0) {
				t->error = 1;
				exit(-1);
			}
			for (i = t->nparams; i < t->nargs; ++i)
				free(t->args[i]);
			t->nargs = t->nparams;
			t->pid = 0;
			t->error = 0;
			t->in = -1;
			t->out = -1;
		}
	}
}

static void
build(char *file)
{
	int tool = toolfor(file), nexttool;
	struct objects *objs = (tool == LD || cflag || kflag) ?
	                       &objout : &objtmp;

	for (; tool < LAST_TOOL; tool = nexttool) {
		switch (tool) {
		case CC1:
			if (Eflag)
				nexttool = LAST_TOOL;
			else
				nexttool = kflag ? TEEIR : CC2;
			break;
		case TEEIR:
			nexttool = CC2;
			break;
		case CC2:
			if (!arch || strcmp(arch, "qbe"))
				nexttool = (Sflag || kflag) ? TEEAS : AS;
			else
				nexttool = kflag ? TEEQBE : QBE;
			break;
		case TEEQBE:
			nexttool = QBE;
			break;
		case QBE:
			nexttool = (Sflag || kflag) ? TEEAS : AS;
			break;
		case TEEAS:
			nexttool = Sflag ? LAST_TOOL : AS;
			break;
		case AS:
			nexttool = LAST_TOOL;
			break;
		default:
			nexttool = LAST_TOOL;
			continue;
		}

		spawn(settool(inittool(tool), file, nexttool));
	}

	validatetools();

	objs->f = newitem(objs->f, objs->n++, outfilename(file, "o"));
}

static void
usage(void)
{
	die("usage: scc [-D def[=val]]... [-U def]... [-I dir]... "
	    "[-L dir]... [-l dir]...\n"
	    "           [-ksw] [-m arch] [-E|-S] [-o outfile] file...\n"
	    "       scc [-D def[=val]]... [-U def]... [-I dir]... "
	    "[-L dir]... [-l dir]...\n"
	    "           [-ksw] [-m arch] [-E|-S] -c file...\n"
	    "       scc [-D def[=val]]... [-U def]... [-I dir]... "
	    "[-L dir]... [-l dir]...\n"
	    "           [-ksw] [-m arch] -c -o outfile file");
}

int
main(int argc, char *argv[])
{
	atexit(terminate);

	arch = getenv("ARCH");

	ARGBEGIN {
	case 'D':
		addarg(CC1, "-D");
		addarg(CC1, EARGF(usage()));
		break;
	case 'E':
		Eflag = 1;
		addarg(CC1, "-E");
		break;
	case 'I':
		addarg(CC1, "-I");
		addarg(CC1, EARGF(usage()));
		break;
	case 'L':
		addarg(LD, "-L");
		addarg(LD, EARGF(usage()));
		break;
	case 'S':
		Sflag = 1;
		break;
	case 'U':
		addarg(CC1, "-U");
		addarg(CC1, EARGF(usage()));
		break;
	case 'c':
		cflag = 1;
		break;
	case 'k':
		kflag = 1;
		break;
	case 'l':
		addarg(LD, "-l");
		addarg(LD, EARGF(usage()));
		break;
	case 'm':
		arch = EARGF(usage());
		break;
	case 'o':
		outfile = xstrdup(EARGF(usage()));
		break;
	case 's':
		sflag = 1;
		break;
	case 'w':
		addarg(CC1, "-w");
		break;
	case '-':
		fprintf(stderr,
		        "scc: ignored parameter --%s\n", EARGF(usage()));
		break;
	default:
		usage();
	} ARGEND

	if (Eflag && (Sflag || kflag) || argc > 1 && cflag && outfile || !argc)
		usage();

	for (; *argv; ++argv)
		build(*argv);

	if (Eflag || Sflag)
		return 0;

	if (!cflag) {
		spawn(settool(inittool(LD), NULL, LAST_TOOL));
		validatetools();
	}

	if (sflag) {
		spawn(settool(inittool(STRIP), NULL, LAST_TOOL));
		validatetools();
	}

	return 0;
}
