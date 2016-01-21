
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include "cc2.h"

#define MAXLINE 200

static void
push(Node * np)
{
}

static Node *
pop(void)
{
}

static void
expr(char *tok)
{
}

static void
stmt(Node *np)
{
}

static void
decl(char *tok)
{
}

void
parse(void)
{
	char line[MAXLINE];
	size_t len;

	for (;;) {
		if (fgets(line, sizeof(line), stdin))
			break;
		if ((len = strlen(line)) == 0 || line[0] == '\n')
			continue;
		if (line[len-1] != '\n')
			error(ELNLINE);
		line[len-1] = '\0';
		switch (*line) {
		case '\t':
			expr(strtok(line, "\t"));
			stmt(pop());
			break;
		default:
			decl(strtok(line, "\t"));
			break;
		}
	}

	if (ferror(stdin))
		error(EFERROR, strerror(errno));
}
