
#include <stddef.h>
#include <stdint.h>

#include "cc.h"

extern void init_keywords(void),
	open_file(const char *file),  init_expr(void);

struct user_opt options;

int
main(int argc, char *argv[])
{
	init_keywords();
	init_expr();
	open_file(NULL);
	for (next(); yytoken != EOFTOK; extdecl());
		/* nothing */;

	return 0;
}
