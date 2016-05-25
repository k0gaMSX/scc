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
static char *argcc1[NARGS];
static char *argcc2[NARGS];

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
cc1(int fd)
{
	pid_t pid;
	char *fmt;
	int r;

	switch (pid = fork()) {
	case -1:
		die("scc: cc1: %s", strerror(errno));
		exit(1);
	case 0:
		dup2(fd, 1);
		fmt = (arch) ? "%s/libexec/scc/cc1-%s" : "%s/libexec/scc/cc1";
		r = snprintf(cmd, sizeof(cmd), fmt, PREFIX, arch);
		if (r == sizeof(cmd))
			die("scc: incorrect prefix\n");
		execv(cmd, argcc1);
		fprintf(stderr, "scc: execv cc1: %s\n", strerror(errno));
		_exit(1);
	default:
		pid_cc1 = pid;
		close(fd);
		break;
	}
}

pid_t
cc2(int fd)
{
	pid_t pid;
	char *fmt;
	int r;

	switch (pid = fork()) {
	case -1:
		die("scc: cc2: %s", strerror(errno));
	case 0:
		dup2(fd, 0);
		fmt = (arch) ? "%s/libexec/scc/cc2-%s" : "%s/libexec/scc/cc2";
		r = snprintf(cmd, sizeof(cmd), fmt, PREFIX, arch);
		if (r == sizeof(cmd))
			die("scc: incorrect prefix");
		execv(cmd, argcc2);
		fprintf(stderr, "scc: execv cc2: %s\n", strerror(errno));
		_exit(1);
	default:
		pid_cc2 = pid;
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
	char *p;
	pid_t pid;

	atexit(terminate);

	if (p = getenv("ARCH"))
		arch = p;

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

	argcc1[0] = "cc1";
	argcc1[1] = *argv;
	argcc2[0] = "cc2";

	cc1(fds[1]);
	cc2(fds[0]);

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
