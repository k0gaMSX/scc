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
	NR_TOOLS,
};

static struct {
	char cmd[FILENAME_MAX];
	char *args[NARGS];
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
settool(int tool, int pipeout)
{
	char *namefmt, *cmdfmt;
	int fds[2], n;
	static int fdin;

	if (tool != QBE) {
		n = snprintf(tools[tool].bin, sizeof(tools[tool].bin),
		             arch ? "%s-%s" : "%s", tools[tool].name, arch);
		if (n < 0 || n >= sizeof(tools[tool].bin))
			die("scc: target tool name too long");

		n = snprintf(tools[tool].cmd, sizeof(tools[tool].cmd),
		             "%s/libexec/scc/%s", PREFIX, tools[tool].bin);
		if (n < 0 || n >= sizeof(tools[tool].cmd))
			die("scc: target tool path too long");
	}

	tools[tool].args[0] = tools[tool].bin;

	if (fdin) {
		tools[tool].in = fdin;
		fdin = 0;
	}
	if (pipeout) {
		if (pipe(fds))
			die("scc: pipe: %s", strerror(errno));
		tools[tool].out = fds[1];
		fdin = fds[0];
	}

	return tool;
}

void
spawn(int tool)
{
	switch (tools[tool].pid = fork()) {
	case -1:
		die("scc: %s: %s", tools[tool].name, strerror(errno));
	case 0:
		if (tools[tool].out)
			dup2(tools[tool].out, 1);
		if (tools[tool].in)
			dup2(tools[tool].in, 0);
		execvp(tools[tool].cmd, tools[tool].args);
		fprintf(stderr, "scc: execv %s: %s\n",
		        tools[tool].cmd, strerror(errno));
		_exit(1);
	default:
		if (tools[tool].in)
			close(tools[tool].in);
		if (tools[tool].out)
			close(tools[tool].out);
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

	tools[CC1].args[1] = *argv;

	spawn(settool(CC1, 1));
	if (!arch || strcmp(arch, "qbe")) {
		spawn(settool(CC2, 0));
	} else {
		spawn(settool(CC2, 1));
		spawn(settool(QBE, 0));
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
