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
	CC2,
	QBE,
	AS,
	TEE,
	NR_TOOLS,
};

static struct tool {
	char  cmd[PATH_MAX];
	char *args[NARGS];
	char  bin[16];
	int   nargs, in, out;
	pid_t pid;
} tools[NR_TOOLS] = {
	[CC1] = { .bin = "cc1", .cmd = PREFIX "/libexec/scc/", },
	[CC2] = { .bin = "cc2", .cmd = PREFIX "/libexec/scc/", },
	[QBE] = { .bin = "qbe", .cmd = "qbe", },
	[AS]  = { .bin = "as",  .cmd = "as", },
	[TEE] = { .bin = "tee", .cmd = "tee", },
};

char *argv0;
static char *arch;
static char *outfiles[NR_TOOLS + 1];
static int failedtool = NR_TOOLS;
static int Eflag, Sflag, kflag;

static void
cleanup(void)
{
	int i;

	for (i = 0; i < NR_TOOLS + 1; ++i) {
		if (outfiles[i]) {
			if (i > failedtool)
				unlink(outfiles[i]);
			free(outfiles[i]);
			outfiles[i] = NULL;
		}
	}
}

static void
terminate(void)
{
	int i;

	for (i = 0; i < NR_TOOLS; ++i) {
		if (tools[i].pid)
			kill(tools[i].pid, SIGTERM);
	}

	cleanup();
}

static int
inittool(int tool)
{
	struct tool *t = &tools[tool];
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
		default:
			break;
		}

		t->args[0] = t->bin;
	}

	return tool;
}

static char *
newfileext(char *name, char *ext)
{
	char *new, *dot;
	size_t newsz, nameln = strlen(name);
	int n;

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
settool(int tool, char *input, int output)
{
	struct tool *t = &tools[tool];
	int fds[2];
	char *ext;
	static int fdin;

	switch (tool) {
	case AS:
		outfiles[output] = newfileext(input, "o");
		t->args[t->nargs] = outfiles[output];
		t->args[3] = NULL;
		break;
	case TEE:
		switch (output) {
		case CC2:
			ext = "ir"; break;
		case QBE:
			ext = "qbe"; break;
		case NR_TOOLS:
			if (!Sflag)
				break;
		case AS:
			ext = "as"; break;
		}
		outfiles[output] = newfileext(input, ext);
		t->args[1] = outfiles[output];
		break;
	default:
		break;
	}

	if (fdin) {
		t->in = fdin;
		fdin = 0;
	} else {
		t->args[t->nargs + 1] = input;
	}

	if (output < NR_TOOLS) {
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
	struct tool *tool = &tools[t];

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
	}

	die("scc: do not recognize filetype of %s", file);
}

static void
build(char *file)
{
	pid_t pid;
	int i, st, tool, out, keepfile;
	static int preout;

	for (tool = toolfor(file); tool < NR_TOOLS; tool = out) {
		keepfile = 0;

		switch (tool) {
		case CC1:
			out = Eflag ? NR_TOOLS : CC2;
			if (!Eflag)
				keepfile = kflag;
			break;
		case CC2:
			if (!arch || strcmp(arch, "qbe")) {
				out = Sflag ? NR_TOOLS : AS;
				keepfile = (Sflag || kflag);
			} else {
				out = QBE;
				keepfile = kflag;
			}
			break;
		case QBE:
			out = Sflag ? NR_TOOLS : AS;
			keepfile = (Sflag || kflag);
			break;
		case AS:
			out = NR_TOOLS;
			break;
		case TEE:
			out = preout;
			break;
		default:
			break;
		}

		if (keepfile) {
			preout = out;
			out = TEE;
		}

		spawn(settool(inittool(tool), file, out));
	}

	for (i = 0; i < NR_TOOLS; ++i) {
		if ((pid = tools[i].pid) == 0)
			continue;
		if (waitpid(pid, &st, 0) < 0) {
			failedtool = i;
			exit(-1);
		}
		tools[i].pid = 0;
		if (!WIFEXITED(st) || WEXITSTATUS(st) != 0) {
			failedtool = i;
			exit(-1);
		}
	}

	cleanup();
}

static void
addarg(int tool, char *arg) {
	struct tool *t = &tools[tool];

	if (t->nargs >= NARGS - 3) /* 3: argv0, filename, NULL terminator */
		die("scc: too many parameters given");

	t->args[++t->nargs] = arg;
}

static void
usage(void)
{
	die("usage: %s [-E|-kS] [-m arch] [-D macro[=val]]... "
	    "[-I dir]... file ...", argv0);
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

	return 0;
}
