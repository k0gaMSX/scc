
#include <stdlib.h>
#include <string.h>
#undef qsort

/*
 * This implementation of qsort is based in the paper
 * "Engineering a Sort Function", by Jon L.Bentley and M. Douglas McIlroy.
 * A lot of different optimizations were removed to make the code simpler.
 */

struct qsort {
	size_t es;
	int (*cmp)(const void *, const void *);
};

static void
swap(char *i, char *j, size_t n)
{
	do {
		char c = *i;
		*i++ = *j;
		*j++ = c;
	} while (--n > 0);
}

static void
xqsort(char *a, size_t n, struct qsort *qs)
{
	size_t j, es = qs->es;
	char *pi, *pj, *pn;

	if (n <= 1)
		return;

	pi = a;
	pn = pj = a + n*es;

	swap(a, a + n/2 * es,  es);
	for (;;) {
		do {
			pi += es;
		} while  (pi < pn && qs->cmp(pi, a) < 0);

		do {
			pj -= es;
		} while (pj > a && qs->cmp(pj, a) > 0);

		if (pj < pi)
			break;
		swap(pi, pj, es);
	}
	swap(a, pj, es);

	j = (pj - a) / es;
	xqsort(a, j, qs);
	xqsort(a + (j+1)*es, n-j-1, qs);
}

void
qsort(void *base, size_t nmemb, size_t size,
      int (*f)(const void *, const void *))
{
	struct qsort qs;

	qs.cmp = f;
	qs.es = size;
	xqsort(base, nmemb, &qs);
}
