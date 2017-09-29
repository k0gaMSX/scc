
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

	AREG_8,		/* class register for B, C, D, E, H, L and A */
};

extern int r8(int reg);