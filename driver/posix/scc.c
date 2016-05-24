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

char *argv0;

#define NARGS 64
static char cmd[FILENAME_MAX];
static char *argcc1[NARGS], *argcc2[NARGS];

static pid_t pid_cc1, pid_cc2;
static char *arch;

static void
terminate(void)
{
	if (pid_cc1)
		kill(pid_cc1, SIGTERM);
	if (pid_cc2)
		kill(pid_cc2, SIGTERM);
}

void
spawn(char *tool, char *args[NARGS], pid_t *pid_tool, int fd, int stdfd)
{
	pid_t pid;
	char archtool[16], *fmt = "%s/libexec/scc/%s";
	int r;

	r = snprintf(archtool, sizeof(archtool),
	             arch ? "%s-%s" : "%s", tool, arch);
	if (r == -1 || r >= sizeof(archtool))
		die("scc: incorrect target arch");

	switch (pid = fork()) {
	case -1:
		die("scc: %s: %s", archtool, strerror(errno));
	case 0:
		dup2(fd, stdfd);
		r = snprintf(cmd, sizeof(cmd), fmt, PREFIX, archtool);
		if (r == - 1 || r >= sizeof(cmd))
			die("scc: incorrect prefix");
		args[0] = archtool;
		execv(cmd, args);
		fprintf(stderr, "scc: execv %s: %s\n", cmd, strerror(errno));
		_exit(1);
	default:
		*pid_tool = pid;
		close(fd);
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
	int fds[2], st, i;
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

	if (pipe(fds))
		die("scc: pipe: %s", strerror(errno));

	argcc1[1] = *argv;
	spawn("cc1", argcc1, &pid_cc1, fds[1], 1);
	spawn("cc2", argcc2, &pid_cc2, fds[0], 0);

	for (i = 0; i < 2; ++i) {
		pid = wait(&st);
		if (pid == pid_cc1)
			pid_cc1 = 0;
		else if (pid == pid_cc2)
			pid_cc2 = 0;
		if (!WIFEXITED(st) || WEXITSTATUS(st) != 0)
			exit(-1);
	}

	return 0;
}
