
enum tflags {
	SIGNF   =    1,
	INTF    =    2,
	STRF    =    8,
	UNIONF  =    16,
	FUNF    =    32,
	INITF   =   128
};

enum op {
	/* types */
	ELLIPSIS = 'E',
	INT8     = 'C',
	INT16    = 'I',
	INT32    = 'W',
	INT64    = 'Q',
	UINT8    = 'K',
	UINT16   = 'N',
	UINT32   = 'Z',
	UINT64   = 'O',
	POINTER  = 'P',
	FUNCTION = 'F',
	VECTOR   = 'V',
	UNION    = 'U',
	STRUCT   = 'S',
	BOOL     = 'B',
	FLOAT    = 'J',
	DOUBLE   = 'D',
	LDOUBLE  = 'H',
	VOID     = '0',
	ONAME    = '"',
	/* kind of operand */
	NONE     = 0,
	MEM      = 'M',
	AUTO     = 'A',
	REG      = 'R',
	/* storage class */
	GLOB     = 'G',
	EXTRN    = 'X',
	PRIVAT   = 'Y',
	LOCAL    = 'T',
	MEMBER   = 'M',
	LABEL    = 'L',
	/* operands */
	OADD     = '+',
	OSUB     = '-',
	OMUL     = '*',
	OMOD     = '%',
	ODIV     = '/',
	OSHL     = 'l',
	OSHR     = 'r',
	OLT      = '<',
	OGT      = '>',
	OLE      = '[',
	OGE      = ']',
	OEQ      = '=',
	ONE      = '!',
	OBAND    = '&',
	OBOR     = '|',
	OBXOR    = '^',
	OCPL     = '~',
	OASSIG   = ':',
	ONEG     = '_',
	OCALL    = 'c',
	OPAR     = 'p',
	OFIELD   = '.',
	OCOMMA   = ',',
	OASK     = '?',
	OCOLON   = ' ',
	OADDR    = '\'',
	OAND     = 'a',
	OOR      = 'b',
	OPTR     = '@',
	OSYM     = 'y',
	OCAST    = 'g',
	OCONST   = '#',
	OSTRING  = '"',
	/*statements */
	OJMP     = 'j',
	ORET     = 'r',
	OBLOOP   = 'b',
	OELOOP   = 'e',
	OCASE    = 'v',
	ODEFAULT = 'f',
	OTABLE   = 't',
	OSWITCH  = 's',
	OEPARS   = '\\',
	OSTMT    = '\t'
};

enum nerrors {
	EIDOVER,       /* identifier overflow */
	EOUTPAR,       /* out pf params */
	ESYNTAX,       /* syntax error */
	ESTACKA,       /* stack unaligned */
	ESTACKO,       /* stack overflow */
	ESTACKU,       /* stack underflow */
	ELNLINE,       /* line too long */
	EFERROR,       /* error reading from file:%s*/
	ENUMERR
};

typedef struct node Node;
typedef struct type Type;
typedef struct symbol Symbol;

struct type {
	TSIZE size;
	TSIZE align;
	char flags;
};

struct symbol {
	Type type;
	unsigned short id;
	unsigned short numid;
	char *name;
	char kind;
	TSIZE off;
	Symbol *next;
	Symbol *h_next;
};

struct node {
	char op;
	Type type;
	union {
		TUINT i;
		char *s;
		Symbol *sym;
	} u;
	Node *left, *right;
	Node *stmt;
};

/* main.c */
extern void error(unsigned nerror, ...);

/* parse.c */
extern void parse(void);

/* optm.c */
extern void optimize(void);

/* cgen.c */
extern void addable(void);
extern void generate(void);

/* peep.c */
extern void peephole(void);

/* code.c */
extern void label(Symbol *sym);
extern void data(Node *np);
extern void allocdata(Type *tp);
extern void writeout(void);

/* node.c */
extern void cleannodes(void);
extern void delnode(Node *np);
extern void deltree(Node *np);
extern Node *newnode(void);

/* symbol.c */
extern Symbol *getsym(int id);
extern void popctx(void);
extern void pushctx(void);
extern void freesym(Symbol *sym);
