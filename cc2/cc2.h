
enum nerrors {
	ELNLINE,       /* line too long */
	EFERROR,       /* error reading from file:%s*/
	ENUMERR
};

typedef struct node Node;

struct node {
	unsigned char op;
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
extern void writeout(void);
