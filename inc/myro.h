
#define MYROHDR_SIZ    48
#define MYROSECT_SIZ   24
#define MYROSYM_SIZ    26
#define MYROREL_SIZ    16

#define MYROMAGIC_SIZ   4
#define MYROMAGIC      "uobj"

#define MYROMAXSEC    254

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

enum myrosecflg {
	MYROSEC_READ  = 1 << 0,
	MYROSEC_WRITE = 1 << 1,
	MYROSEC_EXEC  = 1 << 2,
	MYROSEC_LOAD  = 1 << 3,
	MYROSEC_FILE  = 1 << 4,
	MYROSEC_ABS   = 1 << 5,
};

enum myrosymflg {
	MYROSYM_DEDUP  = 1 << 0,
	MYROSYM_COMMON = 1 << 1,
	MYROSYM_EXTERN = 1 << 2,
	MYROSYM_UNDEF  = 1 << 3,
};

extern int wrmyrohdr(FILE *fp, struct myrohdr *hdr);
extern int wrmyrosec(FILE *fp, struct myrosect *sect);
extern int wrmyrosym(FILE *fp, struct myrosym *sym);
extern int wrmyrorel(FILE *fp, struct myrorel *rel);
extern int rdmyrohdr(FILE *fp, struct myrohdr *hdr);
extern int rdmyrosec(FILE *fp, struct myrosect *sect);
extern int rdmyrosym(FILE *fp, struct myrosym *sym);
extern int rdmyrorel(FILE *fp, struct myrorel *rel);
