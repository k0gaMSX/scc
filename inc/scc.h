#include <stddef.h>

extern int debug;

#ifndef NDEBUG
#define DBG(...) dbg(__VA_ARGS__)
#define DBGON() (debug = 1)
#else
#define DBG(...)
#define DBGON()
#endif

#define TINT        long long
#define TUINT       unsigned long long
#define TFLOAT      double

struct items {
	char **s;
	unsigned n;
};

typedef struct alloc Alloc;

struct arena;
union hdr;

struct alloc {
	size_t size;
	size_t nmemb;
	size_t padding;
	struct arena *arena;
	union hdr *freep;
};


extern void die(const char *fmt, ...);
extern void dbg(const char *fmt, ...);
extern void newitem(struct items *items, char *item);
extern void *xmalloc(size_t size);
extern void *xcalloc(size_t nmemb, size_t size);
extern char *xstrdup(const char *s);
extern void *xrealloc(void *buff, register size_t size);
extern Alloc *alloc(size_t size, size_t nmemb);
extern void dealloc(Alloc *allocp);
extern void *new(Alloc *allocp);
extern void delete(Alloc *allocp, void *p);
extern int casecmp(const char *s1, const char *s2);
