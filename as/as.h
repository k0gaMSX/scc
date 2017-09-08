enum secflags {
	SRELOC,
	SREAD,
	SWRITE,
	SEXEC,
	SFILE,
};

typedef struct ins Ins;
typedef struct op Op;
typedef struct arg Arg;
typedef void Format(Op *, Arg *);
typedef struct section Section;

enum {
	BITS16,
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
	char *args;
};

struct arg {
	int type;
	TUINT val;
};

struct section {
	char *name;
	char *mem;
	int flags;
	TUINT base;
	TUINT max;
	TUINT curpc;
	TUINT pc;
	struct section *next;
};

extern void isections(void);
extern void writeout(char *name);
extern void emit(Section *sec, char *bytes, int nbytes);
extern Section *section(char *name);

extern Section *cursec;
extern int nr_ins;
extern Ins instab[];
extern Op optab[];
extern int pass;
extern TUINT maxaddr;
