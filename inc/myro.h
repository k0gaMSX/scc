
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

extern size_t writehdr(FILE *fp, struct myrohdr *hdr);
extern size_t writesec(FILE *fp, struct myrosect *sect);
extern size_t writesym(FILE *fp, struct myrosym *sym);
extern size_t writerel(FILE *fp, struct myrorel *rel);
