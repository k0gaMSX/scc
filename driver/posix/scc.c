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

struct tool {
	char cmd[FILENAME_MAX];
	char *args[NARGS];
	char bin[16];
	char name[8];
	int in, out;
	pid_t pid;
};

typedef struct tool Tool;

char *argv0;
static Tool cc1 = { .name = "cc1" },
            cc2 = { .name = "cc2" },
            qbe = { .name = "qbe", .bin = "qbe", .cmd = "qbe" };
static char *arch;

static void
terminate(void)
{
	if (cc1.pid)
		kill(cc1.pid, SIGTERM);
	if (cc2.pid)
		kill(cc2.pid, SIGTERM);
	if (qbe.pid)
		kill(qbe.pid, SIGTERM);
}

Tool *
settool(Tool *tool, int pipeout)
{
	char *namefmt, *cmdfmt;
	int fds[2], n;
	static int fdin;

	if (tool != &qbe) {
		n = snprintf(tool->bin, sizeof(tool->bin),
		             arch ? "%s-%s" : "%s", tool->name, arch);
		if (n < 0 || n >= sizeof(tool->bin))
			die("scc: target tool name too long");

		n = snprintf(tool->cmd, sizeof(tool->cmd),
		             "%s/libexec/scc/%s", PREFIX, tool->bin);
		if (n < 0 || n >= sizeof(tool->cmd))
			die("scc: target tool path too long");
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

	return tool;
}

void
spawn(Tool *tool)
{
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
	case 'm':
		arch = EARGF(usage());
		break;
	case '-':
		printf("scc: ignored parameter --%s\n", EARGF(usage()));
		break;
	default:
		usage();
	} ARGEND

	if (!argc)
		die("scc: fatal error: no input files");

	cc1.args[1] = *argv;

	spawn(settool(&cc1, 1));
	if (!arch || strcmp(arch, "qbe")) {
		spawn(settool(&cc2, 0));
	} else {
		spawn(settool(&cc2, 1));
		spawn(settool(&qbe, 0));
	}

	for (i = 0; i < 3; ++i) {
		if ((pid = wait(&st)) < 0)
			break;
		if (pid == cc1.pid)
			cc1.pid = 0;
		else if (pid == cc2.pid)
			cc2.pid = 0;
		else if (pid == qbe.pid)
			qbe.pid = 0;
		if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
			exit(-1);
	}

	return 0;
}
