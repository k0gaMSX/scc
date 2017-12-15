/*
 * First 3 bits of flags in segments and symbols are for the
 * type of segment
 */
enum symflags {
	FREG    = 1 << 0,
	FCOMMON = 1 << 1,
	FEXTERN = 1 << 2,
	FUNDEF  = 1 << 3,
	FDEDUP  = 1 << 4,
	FRELOC  = 1 << 5,
};

enum secflags {
	SREAD  = 1 << 0,
	SWRITE = 1 << 1,
	SEXEC  = 1 << 2,
	SLOAD  = 1 << 3,
	SFILE  = 1 << 4,
	SABS   = 1 << 5,
};

enum endianess {
	BIG_ENDIAN    = -1,
	LITTLE_ENDIAN = 1
};

enum common_args {
	AIMM = 1,
	ASTR,
	AREG,
	AIMM8,
	AIMM16,
	AIMM32,
	AIMM64,
	AINDIR,
	AINDEX,
	ADIRECT,
	AREG_OFF,
	ASYM,
	AMAX,
	AREP = 128
};

#define MAXSYM 63

typedef struct reloc Reloc;
typedef struct ins Ins;
typedef struct op Op;
typedef struct section Section;
typedef struct symbol Symbol;
typedef struct node Node;
typedef struct string String;
typedef void Format(Op *, Node **);

struct string {
	char *buf;
	size_t offset;
};

struct line {
	char *label;
	char *op;
	char *args;
};

struct ins {
	int begin, end;
	char *str;
};

struct reloc {
	size_t offset;
	Symbol *sym;
	unsigned char flags;
	unsigned char size;
	unsigned char nbits;
	unsigned char shift;
};

struct op {
	unsigned char flags;
	char size;
	void (*format)(Op *, Node **);
	unsigned char *bytes;
	unsigned char *args;
};

struct section {
	String name;
	char *mem;
	unsigned char flags;
	unsigned char fill;
	unsigned char aligment;
	unsigned id;
	TUINT base;
	TUINT max;
	TUINT curpc;
	TUINT pc;
	struct section *next;
};

struct symbol {
	String name;
	unsigned char flags;
	unsigned char pass;
	unsigned char argtype;
	TUINT value;
	Section *section;
	struct symbol *next;
	struct symbol *hash;
};

struct node {
	unsigned char op;
	unsigned char addr;
	struct symbol *sym;
	struct node *left;
	struct node *right;
};

/* symbol.c */
extern void isections(void);
extern void emit(char *bytes, int nbytes);
extern Section *setsection(char *name);
extern Symbol *tmpsym(TUINT val);
extern void killtmp(void);
extern int toobig(Node *np, int type);
extern void dumpstab(char *msg);
extern String newstring(char *s);

/* main.c */
extern Symbol *lookup(char *name, int type);
extern Symbol *deflabel(char *name);

/* parser.c */
extern Node **getargs(char *s);
extern void error(char *msg, ...);
/* Avoid errors in files where stdio is not included */
#ifdef stdin
extern int nextline(FILE *fp, struct line *linep);
#endif

/* expr.c */
extern Node *expr(char **s);
extern void deltree(Node *np);

/* proc.c */
extern void iarch(void);
extern int match(Op *op, Node **args);

/* ins.c */
extern char *tobytes(TUINT v, int n, int inc);

/* format.c */
extern void writeout(char *name);
extern void reloc(Symbol *sym,
                  unsigned flags,
                  unsigned size,
                  unsigned nbits,
                  unsigned shift);


/*
 * Definition of global variables
 */
extern Section *cursec, *seclist;
extern int nr_ins;
extern Ins instab[];
extern Op optab[];
extern int pass;
extern TUINT maxaddr;
extern int endian;
extern Symbol *linesym, *symlist;
extern char *infile;
