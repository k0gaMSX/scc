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

#define NARGS 64

enum {
	CC1,
	TEEIR,
	CC2,
	TEEQBE,
	QBE,
	TEEAS,
	AS,
	LD,
	LAST_TOOL,
};

typedef struct tool Tool;
struct tool {
	char  cmd[PATH_MAX];
	char *args[NARGS];
	char  bin[16];
	char *outfile;
	int   nargs, in, out, error;
	pid_t pid;
};

static Tool *tools[] = {
	[CC1]    = &(Tool){ .bin = "cc1", .cmd = PREFIX "/libexec/scc/", },
	[TEEIR]  = &(Tool){ .bin = "tee", .cmd = "tee", },
	[CC2]    = &(Tool){ .bin = "cc2", .cmd = PREFIX "/libexec/scc/", },
	[TEEQBE] = &(Tool){ .bin = "tee", .cmd = "tee", },
	[QBE]    = &(Tool){ .bin = "qbe", .cmd = "qbe", },
	[TEEAS]  = &(Tool){ .bin = "tee", .cmd = "tee", },
	[AS]     = &(Tool){ .bin = "as",  .cmd = "as", },
	[LD]     = &(Tool){ .bin = "gcc", .cmd = "gcc", }, /* TODO replace with ld */
};

char *argv0;
static char *arch;
static char *tmpobjs[NARGS - 2];
static int nobjs;
static int Eflag, Sflag, kflag;

static void
terminate(void)
{
	Tool *t;
	int tool, failed;

	for (tool = 0; tool < LAST_TOOL; ++tool) {
		t = tools[tool];
		if (t->pid) {
			kill(t->pid, SIGTERM);
			if (t->error)
				failed = tool;
			if (tool >= failed && t->outfile)
				unlink(t->outfile);
		}
	}
}

static int
inittool(int tool)
{
	Tool *t = tools[tool];
	size_t binln;
	int n;

	if (!t->args[0]) {
		switch (tool) {
		case CC1:
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
		case AS:
			t->nargs = 2;
			t->args[1] = "-o";
			break;
		case LD:
			t->nargs = 2;
			t->args[1] = "-o";
			break;
		default:
			break;
		}

		t->args[0] = t->bin;
	}

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

static void
addarg(int tool, char *arg) {
	Tool *t = tools[tool];

	if (!(t->nargs < NARGS - 2)) /* 2: argv0, NULL terminator */
		die("scc: too many parameters given");

	t->args[++t->nargs] = arg;
}

static int
settool(int tool, char *infile, int nexttool)
{
	Tool *t = tools[tool];
	int fds[2];
	static int fdin;

	switch (tool) {
	case TEEIR:
		t->outfile = outfilename(infile, "ir");
		t->args[1] = t->outfile;
		break;
	case TEEQBE:
		t->outfile = outfilename(infile, "qbe");
		t->args[1] = t->outfile;
		break;
	case TEEAS:
		t->outfile = outfilename(infile, "as");
		t->args[1] = t->outfile;
		break;
	case AS:
		t->outfile = outfilename(infile, "o");
		t->args[2] = t->outfile;
		break;
	case LD:
		if (!t->outfile)
			t->outfile = xstrdup("a.out");
		t->args[2] = t->outfile;
		break;
	default:
		break;
	}

	if (fdin) {
		t->in = fdin;
		fdin = 0;
	} else {
		t->args[t->nargs + 1] = infile;
	}

	if (nexttool < LAST_TOOL && tool != AS) {
		if (pipe(fds))
			die("scc: pipe: %s", strerror(errno));
		t->out = fds[1];
		fdin = fds[0];
	}

	return tool;
}

static void
spawn(int t)
{
	Tool *tool = tools[t];

	switch (tool->pid = fork()) {
	case -1:
		die("scc: %s: %s", tool->bin, strerror(errno));
	case 0:
		if (tool->out)
			dup2(tool->out, 1);
		if (tool->in)
			dup2(tool->in, 0);
		execvp(tool->cmd, tool->args);
		fprintf(stderr, "scc: execp %s: %s\n",
		        tool->cmd, strerror(errno));
		_exit(1);
	default:
		if (tool->in)
			close(tool->in);
		if (tool->out)
			close(tool->out);
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
	Tool *t;
	int tool, st;
	for (tool = 0; tool < LAST_TOOL; ++tool) {
		t = tools[tool];
		if (t->pid) {
			if (waitpid(t->pid, &st, 0) < 0 ||
			    !WIFEXITED(st) || WEXITSTATUS(st) != 0) {
				t->error = 1;
				exit(-1);
			}
			free(t->outfile);
			t->outfile = NULL;
			t->pid = 0;
			t->error = 0;
		}
	}
}

static void
linkobjs(void)
{
	int i;

	settool(inittool(LD), NULL, LAST_TOOL);

	for (i = 0; tmpobjs[i] && i < nobjs; ++i)
		addarg(LD, tmpobjs[i]);

	spawn(LD);
	validatetools();

	if (!kflag) {
		for (i = 0; i < nobjs; ++i)
			unlink(tmpobjs[i]);
	}

	return;
}

static void
build(char *file)
{
	int tool = toolfor(file), nexttool, argfile = (tool == LD) ? 1 : 0;

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
			nexttool = LD;
			break;
		case LD: /* FALLTHROUGH */
			if (argfile)
				addarg(LD, file);
			else
				tmpobjs[nobjs++] = xstrdup(tools[AS]->outfile);
		default:
			nexttool = LAST_TOOL;
			continue;
		}

		spawn(settool(inittool(tool), file, nexttool));
	}

	validatetools();
}

static void
usage(void)
{
	die("usage: %s [-E|-kS] [-w] [-m arch] [-o binout]\n"
	    "       [-D macro[=val]]... [-I dir]... file...", argv0);
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
	case 'S':
		Sflag = 1;
		break;
	case 'k':
		kflag = 1;
		break;
	case 'm':
		arch = EARGF(usage());
		break;
	case 'o':
		tools[LD]->outfile = EARGF(usage());
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

	if (Eflag && (Sflag || kflag))
		usage();

	if (!argc)
		die("scc: fatal error: no input file");

	for (; *argv; ++argv)
		build(*argv);

	if (!(Eflag || Sflag))
		linkobjs();

	return 0;
}
