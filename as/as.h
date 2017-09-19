/*
 * First 3 bits of flags in segments and symbols are for the
 * type of segment
 */
enum symtype {
	TABS    = 1,
	TTEXT   = 2,
	TBSS    = 3,
	TDATA   = 4,
	TMASK   = 7,
};

enum secflags {
	SRELOC  = 1 << 4,
	SREAD   = 1 << 5,
	SWRITE  = 1 << 6,
	SFILE   = 1 << 7,
};

enum symflags {
	FCOMMON = 1 << 4,
	FLOCAL  = 1 << 5,
	FEXTERN = 1 << 6,
	FUNDEF  = 1 << 7,
};

enum args {
	AIMM = 1,
	AMAX,
        AREP  = 1 << 7,
};

enum endianess {
	BIG_ENDIAN    = -1,
	LITTLE_ENDIAN = 1
};

#define MAXSYM 63

typedef struct ins Ins;
typedef struct op Op;
typedef struct arg Arg;
typedef void Format(Op *, Arg *);
typedef struct section Section;
typedef struct symbol Symbol;
typedef struct node Node;

struct line {
	char *label;
	char *op;
	char *args;
};

struct ins {
	int begin, end;
	char *str;
};

struct op {
	int flags;
	int size;
	void (*format)(Op *, Arg *);
	char *bytes;
	unsigned char *args;
};

struct arg {
	unsigned char type;
	TUINT val;
};

struct section {
	char *name;
	char *mem;
	unsigned char flags;
	TUINT base;
	TUINT max;
	TUINT curpc;
	TUINT pc;
	struct section *next;
};

struct symbol {
	char *name;
	unsigned char flags;
	char pass;
	short desc;
	TUINT value;
	struct symbol *next;
};

struct node {
	char op;
	struct symbol *sym;
	struct node *left;
	struct node *right;
};

/* emit.c */
extern char *pack(TUINT v, int n, int inc);
extern void isections(void);
extern void writeout(char *name);
extern void emit(Section *sec, char *bytes, int nbytes);
extern Section *section(char *name);

/* main.c */
extern Symbol *lookup(char *name);
extern Symbol *deflabel(char *name);

/* parser.c */
extern Arg *getargs(char *s);
extern void error(char *msg, ...);
/* Avoid errors in files where stdio is not included */
#ifdef stdin
extern int nextline(FILE *fp, struct line *linep);
#endif

/* expr.c */
extern Node *expr(void);

/*
 * Definition of global variables
 */
extern Section *cursec;
extern int nr_ins;
extern Ins instab[];
extern Op optab[];
extern int pass;
extern TUINT maxaddr;
extern int endian;
extern Symbol *linesym;
extern char *filename;
