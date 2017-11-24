
#define ARMAGIC       "!<arch>\n"
#define ARMAGIC_SIZ   8

struct arhdr {
	char name[17];
	unsigned long long time;
	int uid;
	int gid;
	int mode;
	unsigned long long size;
};

#define ARHDR_SIZ      60
#define ARNAME_SIZ     16
#define ARMAGIC        "!<arch>\n"
#define ARMAGIC_SIZ    8

extern int wrarhdr(FILE *fp, struct arhdr *hdr);
extern int wrarfile(FILE *fp, struct arhdr *hdr);
