
#define MYROHDR_SIZ    52
#define MYROSECT_SIZ   24
#define MYROSYM_SIZ    24
#define MYROREL_SIZ    20

#define MYROMAGIC_SIZ   4
#define MYROMAGIC      "uobj"

struct myrohdr {
	char magic[4];
	unsigned long format;
	unsigned long long entry;
	unsigned long long strsize;
	unsigned long long secsize;
	unsigned long long symsize;
	unsigned long long relsize;
};

struct myrosect {
	unsigned long name;
	unsigned short flags;
	unsigned char fill;
	unsigned char aligment;
	unsigned long long offset;
	unsigned long long len;
};

struct myrosym {
	unsigned long name;
	unsigned long type;
	unsigned char section;
	unsigned char flags;
	unsigned long long offset;
	unsigned long long len;
};

struct myrorel {
	unsigned long id;
	unsigned char flags;
	unsigned char size;
	unsigned char nbits;
	unsigned char shift;
	unsigned long long offset;
};

extern int wrmyrohdr(FILE *fp, struct myrohdr *hdr);
extern int wrmyrosec(FILE *fp, struct myrosect *sect);
extern int wrmyrosym(FILE *fp, struct myrosym *sym);
extern int wrmyrorel(FILE *fp, struct myrorel *rel);
extern int rdmyrohdr(FILE *fp, struct myrohdr *hdr);
extern int rdmyrosec(FILE *fp, struct myrosect *sect);
extern int rdmyrosym(FILE *fp, struct myrosym *sym);
extern int rdmyrorel(FILE *fp, struct myrorel *rel);
