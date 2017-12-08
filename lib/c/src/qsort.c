
#include <stdlib.h>
#include <string.h>
#undef qsort

/*
 * This implementation of qsort is based in the paper
 * "Engineering a Sort Function", by Jon L.Bentley and M. Douglas McIlroy
 */

struct qsort {
	size_t es;
	int (*cmp)(const void *, const void *);
	void (*swap)(char *i, char *j, size_t n);
};

static void
swapb(char *i, char *j, size_t n)
{
	do {
		char c = *i;
		*i++ = *j;
		*j++ = c;
	} while (--n > 0);
}

static void
swapw(char *i, char *j, size_t n)
{
	int w, *wi = (int *) i, *wj = (int *) j;

	n /= sizeof(w);

	do {
		w = *wi;
		*wi++ = *wj;
		*wj++ = w;
	} while (--n > 0);
}

static void
swapl(char *i, char *j, size_t n)
{
	char *tmp[n];

	memcpy(tmp, i, n);
	memcpy(i, j, n);
	memcpy(j, tmp, n);
}

static char *
med3(char *a, char *b, char *c, int (*cmp)(const void *, const void *))
{
	if (cmp(a, b) < 0) {
		if (cmp(b, c) < 0)
			return b;
		return (cmp(a, c) < 0) ? c : a;
	} else {
		if (cmp(b, c) > 0)
			return b;
		return (cmp(a, c) > 0) ? c : a;
	}
}

static char *
pivot(char *v, size_t n, struct qsort *qs)
{
	char *p1, *pm, *pn; /* 1st, middle, nth */
	int (*cmp)(const void *, const void *);
	size_t s, es = qs->es;
	

	pm = v + n/2 * es;
	if (n < 10)
		return pm;

	cmp = qs->cmp;
	s = n/6 * es;
	p1 = v + s;
	pn = pm + 2*s;

	if (n > 50) {
		s = n/8 * es;
		p1 = med3(p1, p1+s, p1 + 2*s, cmp);
		pm = med3(pm-s, pm, pm+s, cmp);
		pn = med3(pn-2*s, pn-s, pn, cmp);
	}

	return med3(p1, pm, pn, qs->cmp);
}

static void
xqsort(char *a, size_t n, struct qsort *qs)
{
	size_t j, es = qs->es;
	char *pi, *pj, *pn;

tail_recursion:
	if (n <= 1)
		return;

	pi = a;
	pn = pj = a + n*es;

	qs->swap(a, pivot(a, n, qs), es);
	for (;;) {
		do {
			pi += es;
		} while  (pi < pn && qs->cmp(pi, pn) < 0);

		do {
			pj -= es;
		} while (pj > a && qs->cmp(pj, a) > 0);

		if (pj < pi)
			break;
		qs->swap(pi, pj, es);
	}
	qs->swap(a, pj, es);

	/*
	 * We have to recurse over two subarrays. One of them can be
	 * optimized with tail recursion. We select to recurse over
	 * the bigger subarray, because it will allow us to apply a
	 * better divide and conquer strategy.
	 */
	j = (pj - a) / es;
	if (j >= n) {
		xqsort(a, j, qs);
		a += (j+1) * es;
	} else {
		xqsort(a + (j+1) * es, n-j-1, qs);
		n = j;
	}
	goto tail_recursion;
}

void
qsort(void *base, size_t nmemb, size_t size,
      int (*f)(const void *, const void *))
{
	struct qsort qs;

	qs.cmp = f;
	qs.es = size;

	if (size > 7 * sizeof(int))
		qs.swap = swapl;
	else if (size % sizeof(int) == 0)
		qs.swap = swapw;
	else
		qs.swap = swapb;

	xqsort(base, nmemb, &qs);
}
