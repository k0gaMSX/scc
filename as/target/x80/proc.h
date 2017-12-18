
enum args {
	AREG_AF = AMAX,
	AREG_A,
	AREG_F,

	AREG_BC,
	AREG_B,
	AREG_C,

	AREG_HL,
	AREG_H,
	AREG_L,

	AREG_DE,
	AREG_D,
	AREG_E,

	AREG_IX,
	AREG_IXL,
	AREG_IXH,

	AREG_IY,
	AREG_IYL,
	AREG_IYH,

	AREG_R,
	AREG_I,
	AREG_AF_,
	AREG_SP,

	AREG_RCLASS,  /* register class for B, C, D, E, H, L and A */
	AREG_PCLASS,  /* register class for B, C, D, E, IXH, IXL and A */
	AREG_QCLASS,  /* register class for B, C, D, E, IYH, IYL and A */
	AREG_DDCLASS, /* register class for BC, DE, HL and SP */
	AREG_QQCLASS, /* register class for BC, DE, HL and AF */
	AREG_PPCLASS, /* register class for BC, DE, IX and SP */
	AREG_RRCLASS, /* register class for BC, DE, IY and SP */

	AINDEX_IX,    /* (IX+d) */
	AINDEX_IY,    /* (IX+d) */

	AINDER_HL,    /* (HL) */
	AINDER_DE,    /* (DE) */
	AINDER_BC,    /* (BC) */
	AINDER_SP,    /* (SP) */
};

extern int rclass(int reg);
extern int pclass(int reg);
extern int qclass(int reg);
extern int ddclass(int reg);
extern int qqclass(int reg);
extern int ppclass(int reg);
extern int rrclass(int reg);
