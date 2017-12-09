
#include <stdio.h>
#undef freopen

FILE *
freopen(const char * restrict name, const char * restrict mode,
        FILE * restrict fp)
{
	if (fclose(fp) == EOF)
		return NULL;
	return _fpopen(name, mode, fp);
}
