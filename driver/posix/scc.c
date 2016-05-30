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

#define ADDARG(t, p) ((tools[t].args[++tools[t].nargs]) = (p))
#define NARGS 64

enum {
	CC1,
	CC2,
	QBE,
	NR_TOOLS,
};

static struct tool {
	char cmd[PATH_MAX];
	char *args[NARGS];
	int nargs;
	char bin[16];
	char name[8];
	int in, out;
	pid_t pid;
} tools[NR_TOOLS] = {
	[CC1] = { .name = "cc1", },
	[CC2] = { .name = "cc2", },
	[QBE] = { .name = "qbe", .bin = "qbe", .cmd = "qbe", },
};

char *argv0;
static char *arch;
static int Eflag;

static void
terminate(void)
{
	int i;

	for (i = 0; i < NR_TOOLS; ++i) {
		if (tools[i].pid)
			kill(tools[i].pid, SIGTERM);
	}
}

int
settool(int t, int pipeout)
{
	struct tool *tool = &tools[t];
	int fds[2], n;
	static int fdin;

	switch (t) {
	case CC1:
	case CC2:
		n = snprintf(tool->bin, sizeof(tool->bin),
		             arch ? "%s-%s" : "%s", tool->name, arch);
		if (n < 0 || n >= sizeof(tool->bin))
			die("scc: target tool name too long");

		n = snprintf(tool->cmd, sizeof(tool->cmd),
		             "%s/libexec/scc/%s", PREFIX, tool->bin);
		if (n < 0 || n >= sizeof(tool->cmd))
			die("scc: target tool path too long");
		break;
	default:
		break;
	}

	tool->args[0] = tool->bin;

	if (fdin) {
		tool->in = fdin;
		fdin = 0;
	}
	if (pipeout) {
		if (pipe(fds))
			die("scc: pipe: %s", strerror(errno));
		tool->out = fds[1];
		fdin = fds[0];
	}

	return t;
}

void
spawn(int t)
{
	struct tool *tool = &tools[t];

	switch (tool->pid = fork()) {
	case -1:
		die("scc: %s: %s", tool->name, strerror(errno));
	case 0:
		if (tool->out)
			dup2(tool->out, 1);
		if (tool->in)
			dup2(tool->in, 0);
		execvp(tool->cmd, tool->args);
		fprintf(stderr, "scc: execv %s: %s\n",
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

static void
usage(void)
{
	die("usage: %s [-m arch] input ...");
}

int
main(int argc, char *argv[])
{
	int st, i;
	pid_t pid;

	atexit(terminate);

	arch = getenv("ARCH");

	ARGBEGIN {
	case 'E':
		Eflag = 1;
		ADDARG(CC1, "-E");
		break;
	case 'm':
		arch = EARGF(usage());
		break;
	case '-':
		fprintf(stderr, "scc: ignored parameter --%s\n", EARGF(usage()));
		break;
	default:
		usage();
	} ARGEND

	if (!argc)
		die("scc: fatal error: no input files");

	ADDARG(CC1, *argv);

	if (Eflag) {
		spawn(settool(CC1, 0));
	} else {
		spawn(settool(CC1, 1));

		if (!arch || strcmp(arch, "qbe")) {
			spawn(settool(CC2, 0));
		} else {
			spawn(settool(CC2, 1));
			spawn(settool(QBE, 0));
		}
	}

	for (i = 0; i < NR_TOOLS; ++i) {
		if ((pid = wait(&st)) < 0)
			break;
		if (pid == tools[i].pid)
			tools[i].pid = 0;
		if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
			exit(-1);
	}

	return 0;
}
