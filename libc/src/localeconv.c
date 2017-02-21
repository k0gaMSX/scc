#include <locale.h>
#include <limits.h>

struct lconv *
localeconv(void)
{
	static struct lconv lc = { ".", "", "", "", "", "", "", "", "", "",
	                           CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
	                           CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
	                           CHAR_MAX, CHAR_MAX, CHAR_MAX, CHAR_MAX,
	                           CHAR_MAX, CHAR_MAX };
	return &lc;
}
