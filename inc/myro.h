
#define MYROHDR_SIZ    48
#define MYROSECT_SIZ   24
#define MYROSYM_SIZ    24
#define MYROREL_SIZ    20

struct myrohdr {
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

extern int writehdr(FILE *fp, struct myrohdr *hdr);
extern int writesec(FILE *fp, struct myrosect *sect);
extern int writesym(FILE *fp, struct myrosym *sym);
extern int writerel(FILE *fp, struct myrorel *rel);
extern int readhdr(FILE *fp, struct myrohdr *hdr);
extern int readsec(FILE *fp, struct myrosect *sect);
extern int readsym(FILE *fp, struct myrosym *sym);
extern int readrel(FILE *fp, struct myrorel *rel);
