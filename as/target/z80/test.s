	NOP		/ 00
	LD	%BC,64	/ 01 40 00
	LD	(%BC),%A	/ 02
	INC	%BC	/ 03
	INC	%B	/ 04
	DEC	%B	/ 05
	LD	%B,32	/ 06 20
	RLCA		/ 07
	EX	%AF,%AF'	/ 08
	ADD	%HL,%BC	/ 09
	LD	%A,(%BC)	/ 0A
	DEC	%BC	/ 0B
	INC	%C	/ 0C
	DEC	%C	/ 0D
	LD	%C,64	/ 0E 40
	RRCA		/ 0F
/10 n	DJNZ PC + n
	LD	%DE,32	/ 11 20 00
	LD	(%DE),%A	/ 12
	INC	%DE	/ 13
	INC	%D	/ 14
	DEC	%D	/ 15
	LD	%D,64	/ 16 40
	RLA		/ 17
/18 n	JR PC + n
	ADD	%HL,%DE	/ 19
	LD	%A,(%DE)	/ 1A
	DEC	%DE	/ 1B
	INC	%E	/ 1C
	DEC	%E	/ 1D
	LD	%E,64	/ 1E 40
	RRA		/ 1F
/20 n	JR NZ, PC + n
	LD	%HL,32	/ 21 20 00
/22 n n	LD (nn),HL
	INC	%HL	/ 23
	INC	%H	/ 24
	DEC	%H	/ 25
	LD	%H,64	/ 26 40
	DAA		/ 27
/28 n	JR Z, PC + n
	ADD	%HL,%HL	/ 29
/2A n n	LD HL, (nn)
	DEC	%HL	/ 2B
	INC	%L	/ 2C
	DEC	%L	/ 2D
	LD	%L,32	/ 2E 20
	CPL		/ 2F
/30 n	JR NC, PC + n
	LD	%SP,64	/ 31 40 00
/32 n n	LD (nn), A
	INC	%SP	/ 33
	INC	(%HL)	/ 34
	DEC	(%HL)	/ 35
/36 n	LD (HL), n
	SCF		/ 37
/38 n	JR C, PC + n
	ADD	%HL,%SP	/ 39
/3A n n	LD A, (nn)
	DEC	%SP	/ 3B
	INC	%A	/ 3C
	DEC	%A	/ 3D
	LD	%A,32	/ 3E 20
	CCF		/ 3F
	LD	%B,%B	/ 40
	LD	%B,%C	/ 41
	LD	%B,%D	/ 42
	LD	%B,%E	/ 43
	LD	%B,%H	/ 44
	LD	%B,%L	/ 45
	LD	%B,(%HL)	/ 46
	LD	%B,%A	/ 47
	LD	%C,%B	/ 48
	LD	%C,%C	/ 49
	LD	%C,%D	/ 4A
	LD	%C,%E	/ 4B
	LD	%C,%H	/ 4C
	LD	%C,%L	/ 4D
	LD	%C,(%HL)	/ 4E
	LD	%C,%A	/ 4F
	LD	%D,%B	/ 50
	LD	%D,%C	/ 51
	LD	%D,%D	/ 52
	LD	%D,%E	/ 53
	LD	%D,%H	/ 54
	LD	%D,%L	/ 55
	LD	%D,(%HL)	/ 56
	LD	%D,%A	/ 57
	LD	%E,%B	/ 58
	LD	%E,%C	/ 59
	LD	%E,%D	/ 5A
	LD	%E,%E	/ 5B
	LD	%E,%H	/ 5C
	LD	%E,%L	/ 5D
	LD	%E,(%HL)	/ 5E
	LD	%E,%A	/ 5F
	LD	%H,%B	/ 60
	LD	%H,%C	/ 61
	LD	%H,%D	/ 62
	LD	%H,%E	/ 63
	LD	%H,%H	/ 64
	LD	%H,%L	/ 65
	LD	%H,(%HL)	/ 66
	LD	%H,%A	/ 67
	LD	%L,%B	/ 68
	LD	%L,%C	/ 69
	LD	%L,%D	/ 6A
	LD	%L,%E	/ 6B
	LD	%L,%H	/ 6C
	LD	%L,%L	/ 6D
	LD	%L,(%HL)	/ 6E
	LD	%L,%A	/ 6F
	LD	(%HL),%B	/ 70
	LD	(%HL),%C	/ 71
	LD	(%HL),%D	/ 72
	LD	(%HL),%E	/ 73
	LD	(%HL),%H	/ 74
	LD	(%HL),%L	/ 75
	HALT		/ 76
	LD	(%HL),%A	/ 77
	LD	%A,%B	/ 78
	LD	%A,%C	/ 79
	LD	%A,%D	/ 7A
	LD	%A,%E	/ 7B
	LD	%A,%H	/ 7C
	LD	%A,%L	/ 7D
	LD	%A,(%HL)	/ 7E
	LD	%A,%A	/ 7F
	ADD	%A,%B	/ 80
	ADD	%A,%C	/ 81
	ADD	%A,%D	/ 82
	ADD	%A,%E	/ 83
	ADD	%A,%H	/ 84
	ADD	%A,%L	/ 85
	ADD	%A,(%HL)	/ 86
	ADD	%A,%A	/ 87
	ADC	%A,%B	/ 88
	ADC	%A,%C	/ 89
	ADC	%A,%D	/ 8A
	ADC	%A,%E	/ 8B
	ADC	%A,%H	/ 8C
	ADC	%A,%L	/ 8D
	ADC	%A,(%HL)	/ 8E
	ADC	%A,%A	/ 8F
	SUB	%A,%B	/ 90
	SUB	%A,%C	/ 91
	SUB	%A,%D	/ 92
	SUB	%A,%E	/ 93
	SUB	%A,%H	/ 94
	SUB	%A,%L	/ 95
	SUB	%A,(%HL)	/ 96
	SUB	%A,%A	/ 97
	SBC	%A,%B	/ 98
	SBC	%A,%C	/ 99
	SBC	%A,%D	/ 9A
	SBC	%A,%E	/ 9B
	SBC	%A,%H	/ 9C
	SBC	%A,%L	/ 9D
	SBC	%A,(%HL)	/ 9E
	SBC	%A,%A	/ 9F
	AND	%A,%B	/ A0
	AND	%A,%C	/ A1
	AND	%A,%D	/ A2
	AND	%A,%E	/ A3
	AND	%A,%H	/ A4
	AND	%A,%L	/ A5
	AND	%A,(%HL)	/ A6
	AND	%A,%A	/ A7
	XOR	%A,%B	/ A8
	XOR	%A,%C	/ A9
	XOR	%A,%D	/ AA
	XOR	%A,%E	/ AB
	XOR	%A,%H	/ AC
	XOR	%A,%L	/ AD
	XOR	%A,(%HL)	/ AE
	XOR	%A,%A	/ AF
	OR	%A,%B	/ B0
	OR	%A,%C	/ B1
	OR	%A,%D	/ B2
	OR	%A,%E	/ B3
	OR	%A,%H	/ B4
	OR	%A,%L	/ B5
	OR	%A,(%HL)	/ B6
	OR	%A,%A	/ B7
	CP	%A,%B	/ B8
	CP	%A,%C	/ B9
	CP	%A,%D	/ BA
	CP	%A,%E	/ BB
	CP	%A,%H	/ BC
	CP	%A,%L	/ BD
	CP	%A,(%HL)	/ BE
	CP	%A,%A	/ BF
/C0	RET NZ
	POP	%BC	/ C1
/C2 n n	JP NZ, nn
/C3 n n	JP nn
/C4 n n	CALL NZ, nn
	PUSH	%BC	/ C5
	ADD	%A,32	/ C6 20
/C7	RST 0h
/C8	RET Z
	RET		/ C9
/CA n n	JP Z, nn
	RLC	%B	/ CB 00
	RLC	%C	/ CB 01
	RLC	%D	/ CB 02
	RLC	%E	/ CB 03
	RLC	%H	/ CB 04
	RLC	%L	/ CB 05
	RLC	(%HL)	/ CB 06
	RLC	%A	/ CB 07
	RRC	%B	/ CB 08
	RRC	%C	/ CB 09
	RRC	%D	/ CB 0A
	RRC	%E	/ CB 0B
	RRC	%H	/ CB 0C
	RRC	%L	/ CB 0D
	RRC	(%HL)	/ CB 0E
	RRC	%A	/ CB 0F
	RL	%B	/ CB 10
	RL	%C	/ CB 11
	RL	%D	/ CB 12
	RL	%E	/ CB 13
	RL	%H	/ CB 14
	RL	%L	/ CB 15
	RL	(%HL)	/ CB 16
	RL	%A	/ CB 17
	RR	%B	/ CB 18
	RR	%C	/ CB 19
	RR	%D	/ CB 1A
	RR	%E	/ CB 1B
	RR	%H	/ CB 1C
	RR	%L	/ CB 1D
	RR	(%HL)	/ CB 1E
	RR	%A	/ CB 1F
	SLA	%B	/ CB 20
	SLA	%C	/ CB 21
	SLA	%D	/ CB 22
	SLA	%E	/ CB 23
	SLA	%H	/ CB 24
	SLA	%L	/ CB 25
	SLA	(%HL)	/ CB 26
	SLA	%A	/ CB 27
	SRA	%B	/ CB 28
	SRA	%C	/ CB 29
	SRA	%D	/ CB 2A
	SRA	%E	/ CB 2B
	SRA	%H	/ CB 2C
	SRA	%L	/ CB 2D
	SRA	(%HL)	/ CB 2E
	SRA	%A	/ CB 2F
	SLL	%B	/ CB 30
	SLL	%C	/ CB 31
	SLL	%D	/ CB 32
	SLL	%E	/ CB 33
	SLL	%H	/ CB 34
	SLL	%L	/ CB 35
	SLL	(%HL)	/ CB 36
	SLL	%A	/ CB 37
	SRL	%B	/ CB 38
	SRL	%C	/ CB 39
	SRL	%D	/ CB 3A
	SRL	%E	/ CB 3B
	SRL	%H	/ CB 3C
	SRL	%L	/ CB 3D
	SRL	(%HL)	/ CB 3E
	SRL	%A	/ CB 3F
/CB40	BIT 0, B
/CB41	BIT 0, C
/CB42	BIT 0, D
/CB43	BIT 0, E
/CB44 	BIT 0, H
/CB45 	BIT 0, L
/CB46 	BIT 0, (HL)
/CB47 	BIT 0, A
/CB48	BIT 1, B
/CB49 	BIT 1, C
/CB4A 	BIT 1, D
/CB4B 	BIT 1, E
/CB4C 	BIT 1, H
/CB4D 	BIT 1, L
/CB4E 	BIT 1, (HL)
/CB4F 	BIT 1, A
/CB50	BIT 2, B
/CB51 	BIT 2, C
/CB52 	BIT 2, D
/CB53 	BIT 2, E
/CB54 	BIT 2, H
/CB55 	BIT 2, L
/CB56 	BIT 2, (HL)
/CB57 	BIT 2, A
/CB58	BIT 3, B
/CB59 	BIT 3, C
/CB5A 	BIT 3, D
/CB5B 	BIT 3, E
/CB5C 	BIT 3, H
/CB5D 	BIT 3, L
/CB5E 	BIT 3, (HL)
/CB5F 	BIT 3, A
/CB60	BIT 4, B
/CB61 	BIT 4, C
/CB62 	BIT 4, D
/CB63 	BIT 4, E
/CB64 	BIT 4, H
/CB65 	BIT 4, L
/CB66 	BIT 4, (HL)
/CB67 	BIT 4, A
/CB68	BIT 5, B
/CB69 	BIT 5, C
/CB6A 	BIT 5, D
/CB6B 	BIT 5, E
/CB6C 	BIT 5, H
/CB6D 	BIT 5, L
/CB6E 	BIT 5, (HL)
/CB6F 	BIT 5, A
/CB70	BIT 6, B
/CB71 	BIT 6, C
/CB72 	BIT 6, D
/CB73 	BIT 6, E
/CB74 	BIT 6, H
/CB75 	BIT 6, L
/CB76 	BIT 6, (HL)
/CB77 	BIT 6, A
/CB78	BIT 7, B
/CB79 	BIT 7, C
/CB7A 	BIT 7, D
/CB7B 	BIT 7, E
/CB7C 	BIT 7, H
/CB7D 	BIT 7, L
/CB7E 	BIT 7, (HL)
/CB7F 	BIT 7, A
/CB80	RES 0, B
/CB81	RES 0, C
/CB82	RES 0, D
/CB83	RES 0, E
/CB84 	RES 0, H
/CB85 	RES 0, L
/CB86 	RES 0, (HL)
/CB87 	RES 0, A
/CB88	RES 1, B
/CB89 	RES 1, C
/CB8A 	RES 1, D
/CB8B 	RES 1, E
/CB8C 	RES 1, H
/CB8D 	RES 1, L
/CB8E 	RES 1, (HL)
/CB8F 	RES 1, A
/CB90	RES 2, B
/CB91 	RES 2, C
/CB92 	RES 2, D
/CB93 	RES 2, E
/CB94 	RES 2, H
/CB95 	RES 2, L
/CB96 	RES 2, (HL)
/CB97 	RES 2, A
/CB98	RES 3, B
/CB99 	RES 3, C
/CB9A 	RES 3, D
/CB9B 	RES 3, E
/CB9C 	RES 3, H
/CB9D 	RES 3, L
/CB9E 	RES 3, (HL)
/CB9F 	RES 3, A
/CBA0	RES 4, B
/CBA1 	RES 4, C
/CBA2 	RES 4, D
/CBA3 	RES 4, E
/CBA4 	RES 4, H
/CBA5 	RES 4, L
/CBA6 	RES 4, (HL)
/CBA7 	RES 4, A
/CBA8	RES 5, B
/CBA9 	RES 5, C
/CBAA 	RES 5, D
/CBAB 	RES 5, E
/CBAC 	RES 5, H
/CBAD 	RES 5, L
/CBAE 	RES 5, (HL)
/CBAF 	RES 5, A
/CBB0	RES 6, B
/CBB1 	RES 6, C
/CBB2 	RES 6, D
/CBB3 	RES 6, E
/CBB4 	RES 6, H
/CBB5 	RES 6, L
/CBB6 	RES 6, (HL)
/CBB7 	RES 6, A
/CBB8	RES 7, B
/CBB9 	RES 7, C
/CBBA 	RES 7, D
/CBBB 	RES 7, E
/CBBC 	RES 7, H
/CBBD 	RES 7, L
/CBBE 	RES 7, (HL)
/CBBF 	RES 7, A
/CBC0	SET 0, B
/CBC1	SET 0, C
/CBC2	SET 0, D
/CBC3	SET 0, E
/CBC4 	SET 0, H
/CBC5 	SET 0, L
/CBC6 	SET 0, (HL)
/CBC7 	SET 0, A
/CBC8	SET 1, B
/CBC9 	SET 1, C
/CBCA 	SET 1, D
/CBCB 	SET 1, E
/CBCC 	SET 1, H
/CBCD 	SET 1, L
/CBCE 	SET 1, (HL)
/CBCF 	SET 1, A
/CBD0	SET 2, B
/CBD1 	SET 2, C
/CBD2 	SET 2, D
/CBD3 	SET 2, E
/CBD4 	SET 2, H
/CBD5 	SET 2, L
/CBD6 	SET 2, (HL)
/CBD7 	SET 2, A
/CBD8	SET 3, B
/CBD9 	SET 3, C
/CBDA 	SET 3, D
/CBDB 	SET 3, E
/CBDC 	SET 3, H
/CBDD 	SET 3, L
/CBDE 	SET 3, (HL)
/CBDF 	SET 3, A
/CBE0	SET 4, B
/CBE1 	SET 4, C
/CBE2 	SET 4, D
/CBE3 	SET 4, E
/CBE4 	SET 4, H
/CBE5 	SET 4, L
/CBE6 	SET 4, (HL)
/CBE7 	SET 4, A
/CBE8	SET 5, B
/CBE9 	SET 5, C
/CBEA 	SET 5, D
/CBEB 	SET 5, E
/CBEC 	SET 5, H
/CBED 	SET 5, L
/CBEE 	SET 5, (HL)
/CBEF 	SET 5, A
/CBF0	SET 6, B
/CBF1 	SET 6, C
/CBF2 	SET 6, D
/CBF3 	SET 6, E
/CBF4 	SET 6, H
/CBF5 	SET 6, L
/CBF6 	SET 6, (HL)
/CBF7 	SET 6, A
/CBF8	SET 7, B
/CBF9 	SET 7, C
/CBFA 	SET 7, D
/CBFB 	SET 7, E
/CBFC 	SET 7, H
/CBFD 	SET 7, L
/CBFE 	SET 7, (HL)
/CBFF 	SET 7, A
/CC n n	CALL Z, nn
/CD n n	CALL nn
	ADC	%A,64	/ CE 40
/CF	RST 8h
/D0	RET NC
	POP	%DE	/ D1
/D2 n n	JP NC, nn
/D3 n	OUT (n), A
/D4 n n	CALL NC, nn
	PUSH	%DE	/ D5
	SUB	%A,32	/ D6 20
/D7	RST 10h
/D8	RETC
	EXX		/ D9
/DA n n	JP C, nn
/DB n	IN A, (n)
/DC n n	CALL C, nn
	ADD	%IX,%BC	/ DD 09
	ADD	%IX,%DE	/ DD 19
	LD	%IX,64	/ DD 21 40 00
/DD22 n n	LD (nn), IX
	INC	%IX	/ DD 23
	INC	%IXH	/ DD 24
	DEC	%IXH	/ DD 25
	LD	%IXH,32	/ DD 26 20
	ADD	%IX,%IX	/ DD 29
/DD2A n n	LD IX, (nn)
	DEC	%IX	/ DD 2B
	INC	%IXL	/ DD 2C
	DEC	%IXL	/ DD 2D
	LD	%IXL,64	/ DD 2E 40
/DD34 d	INC (IX + d)
/DD35 d	DEC (IX + d)
/DD36 d n	LD (IX + d), n
	ADD	%IX,%SP	/ DD 39
	LD	%B,%IXH	/ DD 44
	LD	%B,%IXL	/ DD 45
/DD46 d	LD B, (IX + d)
	LD	%C,%IXH	/ DD 4C
	LD	%C,%IXL	/ DD 4D
/DD4E d	LD C, (IX + d)
	LD	%D,%IXH	/ DD 54
	LD	%D,%IXL	/ DD 55
/DD56 d	LD D, (IX + d)
	LD	%E,%IXH	/ DD 5C
	LD	%E,%IXL	/ DD 5D
/DD5E d	LD E, (IX + d)
	LD	%IXH,%B	/ DD 60
	LD	%IXH,%C	/ DD 61
	LD	%IXH,%D	/ DD 62
	LD	%IXH,%E	/ DD 63
	LD	%IXH,%IXH	/ DD 64
	LD	%IXH,%IXL	/ DD 65
/DD66 d	LD H, (IX + d)
	LD	%IXH,%A	/ DD 67
	LD	%IXL,%B	/ DD 68
	LD	%IXL,%C	/ DD 69
	LD	%IXL,%D	/ DD 6A
	LD	%IXL,%E	/ DD 6B
	LD	%IXL,%IXH	/ DD 6C
	LD	%IXL,%IXL	/ DD 6D
/DD6E d	LD L, (IX + d)
	LD	%IXL,%A	/ DD 6F
/DD70 d	LD (IX + d), B
/DD71 d	LD (IX + d), C
/DD72 d	LD (IX + d), D
/DD73 d	LD (IX + d), E
/DD74 d	LD (IX + d), H
/DD75 d	LD (IX + d), L
/DD77 d	LD (IX + d), A
/DD7C	LD A, IXH*
/DD7D	LD A, IXL*
/DD7E d	LD A, (IX + d)
/DD84	ADD A, IXH*
/DD85	ADD A, IXL*
/DD86 d	ADD A, (IX + d)
/DD8C	ADC A, IXH*
/DD8D	ADC A, IXL*
/DD8E d	ADC A, (IX + d)
/DD94	SUB IXH*
/DD95	SUB IXL*
/DD96 d	SUB (IX + d)
/DD9C	SBC A, IXH*
/DD9D	SBC A, IXL*
/DD9E d	SBC A, (IX + d)
/DDA4	AND IXH*
/DDA5	AND IXL*
/DDA6 d	AND (IX + d)
/DDAC	XOR IXH*
/DDAD	XOR IXL*
/DDAE d	XOR (IX + d)
/DDB4	OR IXH*
/DDB5	OR IXL*
/DDB6 d	OR (IX + d)
/DDBC	CP IXH*
/DDBD	CP IXL*
/DDBE d	CP (IX + d)
/DDCB d 00	LD B, RLC (IX + d)*
/DDCB d 01	LD C, RLC (IX + d)*
/DDCB d 02	LD D, RLC (IX + d)*
/DDCB d 03	LD E, RLC (IX + d)*
/DDCB d 04	LD H, RLC (IX + d)*
/DDCB d 05	LD L, RLC (IX + d)*
/DDCB d 06	RLC (IX + d)
/DDCB d 07	LD A, RLC (IX + d)*
/DDCB d 08	LD B, RRC (IX + d)*
/DDCB d 09	LD C, RRC (IX + d)*
/DDCB d 0A	LD D, RRC (IX + d)*
/DDCB d 0B	LD E, RRC (IX + d)*
/DDCB d 0C	LD H, RRC (IX + d)*
/DDCB d 0D	LD L, RRC (IX + d)*
/DDCB d 0E	RRC (IX + d)
/DDCB d 0F	LD A, RRC (IX + d)*
/DDCB d 10	LD B, RL (IX + d)*
/DDCB d 11	LD C, RL (IX + d)*
/DDCB d 12	LD D, RL (IX + d)*
/DDCB d 13	LD E, RL (IX + d)*
/DDCB d 14	LD H, RL (IX + d)*
/DDCB d 15	LD L, RL (IX + d)*
/DDCB d 16	RL (IX + d)
/DDCB d 17	LD A, RL (IX + d)*
/DDCB d 18	LD B, RR (IX + d)*
/DDCB d 19	LD C, RR (IX + d)*
/DDCB d 1A	LD D, RR (IX + d)*
/DDCB d 1B	LD E, RR (IX + d)*
/DDCB d 1C	LD H, RR (IX + d)*
/DDCB d 1D	LD L, RR (IX + d)*
/DDCB d 1E	RR (IX + d)
/DDCB d 1F	LD A, RR (IX + d)*
/DDCB d 20	LD B, SLA (IX + d)*
/DDCB d 21	LD C, SLA (IX + d)*
/DDCB d 22	LD D, SLA (IX + d)*
/DDCB d 23	LD E, SLA (IX + d)*
/DDCB d 24	LD H, SLA (IX + d)*
/DDCB d 25	LD L, SLA (IX + d)*
/DDCB d 26	SLA (IX + d)
/DDCB d 27	LD A, SLA (IX + d)*
/DDCB d 28	LD B, SRA (IX + d)*
/DDCB d 29	LD C, SRA (IX + d)*
/DDCB d 2A	LD D, SRA (IX + d)*
/DDCB d 2B	LD E, SRA (IX + d)*
/DDCB d 2C	LD H, SRA (IX + d)*
/DDCB d 2D	LD L, SRA (IX + d)*
/DDCB d 2E	SRA (IX + d)
/DDCB d 2F	LD A, SRA (IX + d)*
/DDCB d 30	LD B, SLL (IX + d)*
/DDCB d 31	LD C, SLL (IX + d)*
/DDCB d 32	LD D, SLL (IX + d)*
/DDCB d 33	LD E, SLL (IX + d)*
/DDCB d 34	LD H, SLL (IX + d)*
/DDCB d 35	LD L, SLL (IX + d)*
/DDCB d 36	SLL (IX + d)*
/DDCB d 37	LD A, SLL (IX + d)*
/DDCB d 38	LD B, SRL (IX + d)*
/DDCB d 39	LD C, SRL (IX + d)*
/DDCB d 3A	LD D, SRL (IX + d)*
/DDCB d 3B	LD E, SRL (IX + d)*
/DDCB d 3C	LD H, SRL (IX + d)*
/DDCB d 3D	LD L, SRL (IX + d)*
/DDCB d 3E	SRL (IX + d)
/DDCB d 3F	LD A, SRL (IX + d)*
/DDCB d 46	BIT 0, (IX + d)
/DDCB d 4E	BIT 1, (IX + d)
/DDCB d 56	BIT 2, (IX + d)
/DDCB d 5E	BIT 3, (IX + d)
/DDCB d 66	BIT 4, (IX + d)
/DDCB d 6E	BIT 5, (IX + d)
/DDCB d 76	BIT 6, (IX + d)
/DDCB d 7E	BIT 7, (IX + d)
/DDCB d 80	LD B, RES 0, (IX + d)*
/DDCB d 81	LD C, RES 0, (IX + d)*
/DDCB d 82	LD D, RES 0, (IX + d)*
/DDCB d 83	LD E, RES 0, (IX + d)*
/DDCB d 84	LD H, RES 0, (IX + d)*
/DDCB d 85	LD L, RES 0, (IX + d)*
/DDCB d 86	RES 0, (IX + d)
/DDCB d 87	LD A, RES 0, (IX + d)*
/DDCB d 88	LD B, RES 1, (IX + d)*
/DDCB d 89	LD C, RES 1, (IX + d)*
/DDCB d 8A	LD D, RES 1, (IX + d)*
/DDCB d 8B	LD E, RES 1, (IX + d)*
/DDCB d 8C	LD H, RES 1, (IX + d)*
/DDCB d 8D	LD L, RES 1, (IX + d)*
/DDCB d 8E	RES 1, (IX + d)
/DDCB d 8F	LD A, RES 1, (IX + d)*
/DDCB d 90	LD B, RES 2, (IX + d)*
/DDCB d 91	LD C, RES 2, (IX + d)*
/DDCB d 92	LD D, RES 2, (IX + d)*
/DDCB d 93	LD E, RES 2, (IX + d)*
/DDCB d 94	LD H, RES 2, (IX + d)*
/DDCB d 95	LD L, RES 2, (IX + d)*
/DDCB d 96	RES 2, (IX + d)
/DDCB d 97	LD A, RES 2, (IX + d)*
/DDCB d 98	LD B, RES 3, (IX + d)*
/DDCB d 99	LD C, RES 3, (IX + d)*
/DDCB d 9A	LD D, RES 3, (IX + d)*
/DDCB d 9B	LD E, RES 3, (IX + d)*
/DDCB d 9C	LD H, RES 3, (IX + d)*
/DDCB d 9D	LD L, RES 3, (IX + d)*
/DDCB d 9E	RES 3, (IX + d)
/DDCB d 9F	LD A, RES 3, (IX + d)*
/DDCB d A0	LD B, RES 4, (IX + d)*
/DCB d A1	LD C, RES 4, (IX + d)*
/DDCB d A2	LD D, RES 4, (IX + d)*
/DDCB d A3	LD E, RES 4, (IX + d)*
/DDCB d A4	LD H, RES 4, (IX + d)*
/DDCB d A5	LD L, RES 4, (IX + d)*
/DDCB d A6	RES 4, (IX + d)
/DDCB d A7	LD A, RES 4, (IX + d)*
/DDCB d A8	LD B, RES 5, (IX + d)*
/DDCB d A9	LD C, RES 5, (IX + d)*
/DDCB d AA	LD D, RES 5, (IX + d)*
/DDCB d AB	LD E, RES 5, (IX + d)*
/DDCB d AC	LD H, RES 5, (IX + d)*
/DDCB d AD	LD L, RES 5, (IX + d)*
/DDCB d AE	RES 5, (IX + d)
/DDCB d AF	LD A, RES 5, (IX + d)*
/DDCB d B0	LD B, RES 6, (IX + d)*
/DDCB d B1	LD C, RES 6, (IX + d)*
/DDCB d B2	LD D, RES 6, (IX + d)*
/DDCB d B3	LD E, RES 6, (IX + d)*
/DDCB d B4	LD H, RES 6, (IX + d)*
/DDCB d B5	LD L, RES 6, (IX + d)*
/DDCB d B6	RES 6, (IX + d)
/DDCB d B7	LD A, RES 6, (IX + d)*
/DDCB d B8	LD B, RES 7, (IX + d)*
/DDCB d B9	LD C, RES 7, (IX + d)*
/DDCB d BA	LD D, RES 7, (IX + d)*
/DDCB d BB	LD E, RES 7, (IX + d)*
/DDCB d BC	LD H, RES 7, (IX + d)*
/DDCB d BD	LD L, RES 7, (IX + d)*
/DDCB d BE	RES 7, (IX + d)
/DDCB d BF	LD A, RES 7, (IX + d)*
/DDCB d C0	LD B, SET 0, (IX + d)*
/DDCB d C1	LD C, SET 0, (IX + d)*
/DDCB d C2	LD D, SET 0, (IX + d)*
/DDCB d C3	LD E, SET 0, (IX + d)*
/DDCB d C4	LD H, SET 0, (IX + d)*
/DDCB d C5	LD L, SET 0, (IX + d)*
/DDCB d C6	SET 0, (IX + d)
/DDCB d C7	LD A, SET 0, (IX + d)*
/DDCB d C8	LD B, SET 1, (IX + d)*
/DDCB d C9	LD C, SET 1, (IX + d)*
/DDCB d CA	LD D, SET 1, (IX + d)*
/DDCB d CB	LD E, SET 1, (IX + d)*
/DDCB d CC	LD H, SET 1, (IX + d)*
/DDCB d CD	LD L, SET 1, (IX + d)*
/DDCB d CE	SET 1, (IX + d)
/DDCB d CF	LD A, SET 1, (IX + d)*
/DDCB d D0	LD B, SET 2, (IX + d)*
/DDCB d D1	LD C, SET 2, (IX + d)*
/DDCB d D2	LD D, SET 2, (IX + d)*
/DDCB d D3	LD E, SET 2, (IX + d)*
/DDCB d D4	LD H, SET 2, (IX + d)*
/DDCB d D5	LD L, SET 2, (IX + d)*
/DDCB d D6	SET 2, (IX + d)
/DDCB d D7	LD A, SET 2, (IX + d)*
/DDCB d D8	LD B, SET 3, (IX + d)*
/DDCB d D9	LD C, SET 3, (IX + d)*
/DDCB d DA	LD D, SET 3, (IX + d)*
/DDCB d DB	LD E, SET 3, (IX + d)*
/DDCB d DC	LD H, SET 3, (IX + d)*
/DDCB d DD	LD L, SET 3, (IX + d)*
/DDCB d DE	SET 3, (IX + d)
/DDCB d DF	LD A, SET 3, (IX + d)*
/DDCB d E0	LD B, SET 4, (IX + d)*
/DDCB d E1	LD C, SET 4, (IX + d)*
/DDCB d E2	LD D, SET 4, (IX + d)*
/DDCB d E3	LD E, SET 4, (IX + d)*
/DDCB d E4	LD H, SET 4, (IX + d)*
/DDCB d E5	LD L, SET 4, (IX + d)*
/DDCB d E6	SET 4, (IX + d)
/DDCB d E7	LD A, SET 4, (IX + d)*
/DDCB d E8	LD B, SET 5, (IX + d)*
/DDCB d E9	LD C, SET 5, (IX + d)*
/DDCB d EA	LD D, SET 5, (IX + d)*
/DDCB d EB	LD E, SET 5, (IX + d)*
/DDCB d EC	LD H, SET 5, (IX + d)*
/DDCB d ED	LD L, SET 5, (IX + d)*
/DDCB d EE	SET 5, (IX + d)
/DDCB d EF	LD A, SET 5, (IX + d)*
/DDCB d F0	LD B, SET 6, (IX + d)*
/DDCB d F1	LD C, SET 6, (IX + d)*
/DDCB d F2	LD D, SET 6, (IX + d)*
/DDCB d F3	LD E, SET 6, (IX + d)*
/DDCB d F4	LD H, SET 6, (IX + d)*
/DDCB d F5	LD L, SET 6, (IX + d)*
/DDCB d F6	SET 6, (IX + d)
/DDCB d F7	LD A, SET 6, (IX + d)*
/DDCB d F8	LD B, SET 7, (IX + d)*
/DDCB d F9	LD C, SET 7, (IX + d)*
/DDCB d FA	LD D, SET 7, (IX + d)*
/DDCB d FB	LD E, SET 7, (IX + d)*
/DDCB d FC	LD H, SET 7, (IX + d)*
/DDCB d FD	LD L, SET 7, (IX + d)*
/DDCB d FE	SET 7, (IX + d)
/DDCB d FF	LD A, SET 7, (IX + d)*
	POP	%IX	/ DD E1
	EX	(%SP),%IX	/ DD E3
	PUSH	%IX	/ DD E5
/	JP	(%IX)	/ DD E9
	LD	%SP,%IX	/ DD F9
/DE n	SBC A, n
/DF	RST 18h
/E0	RET PO
	POP	%HL	/ E1
/E2 n n	JP PO, nn
	EX	(%SP),%HL	/ E3
/E4 n n	CALL PO, nn
	PUSH	%HL	/ E5
/E6 n	AND n
/E7	RST 20h
/E8	RET PE
/E9	JP (HL)
/EA n n	JP PE, (nn)
	EX	%DE,%HL	/ EB
/EC n n	CALL PE, nn
/ED40	IN B, (C)
/ED41	OUT (C), B
/ED42	SBC HL, BC
/ED43 n n	LD (nn), BC
	NEG		/ ED 44
	RETN		/ ED 45
/ED46	IM 0
	LD	%I,%A	/ ED 47
/ED48	IN C, (C)
/ED49	OUT (C), C
/ED4A	ADC HL, BC
/ED4B n n	LD BC, (nn)
	RETI		/ ED 4D
/ED4E	IM 0/1*
/ED4F	LD R, A
/ED50	IN D, (C)
/ED51	OUT (C), D
/ED52	SBC	%HL,%DE	/ ED 52
/ED53 n n	LD (nn), DE
/ED56	IM 1
	LD	%A,%I	/ ED 57
/ED58	IN E, (C)
/ED59	OUT (C), E
	ADC	%HL,%DE	/ ED 5A
/ED5B n n	LD DE, (nn)
/ED5E	IM 2
/ED5F	LD A, R
/ED60	IN H, (C)
/ED61	OUT (C), H
/ED62	SBC	%HL,%HL	/ ED 62
/ED63 n n	LD (nn), HL
/ED66	IM 0*
	RRD		/ ED 67
/ED68	IN L, (C)
/ED69	OUT (C), L
	ADC	%HL,%HL	/ ED 6A
/ED6B n n	LD HL, (nn)
/ED6E	IM 0/1*
	RLD		/ ED 6F
/ED70	IN (C)* / IN F, (C)*
/ED71	OUT (C), 0*
	SBC	%HL,%SP	/ ED 72
/ED73 n n	LD (nn), SP	
/ED76	IM 1*
/ED78	IN A, (C)
/ED79	OUT (C), A
	ADC	%HL,%SP	/ ED 7A
/ED7B n n	LD SP, (nn)
/ED7E	IM 2*
	LDI		/ ED A0
	CPI		/ ED A1
	INI		/ ED A2
	OUTI		/ ED A3
	LDD		/ ED A8
	CPD		/ ED A9
	IND		/ ED AA
	OUTD		/ ED AB
	LDIR		/ ED B0
	CPIR		/ ED B1
	INIR		/ ED B2
	OTIR		/ ED B3
	LDDR		/ ED B8
	CPDR		/ ED B9
	INDR		/ ED BA
	OTDR		/ ED BB
/EE n	XOR n
/EF	RST 28h
/F0	RET P
	POP	%AF	/ F1
/F2 n n	JP P, nn
	DI		/ F3
/F4 n n	CALL P, nn
	PUSH	%AF	/ F5
/F6 n	OR n
/F7	RST 30h
/F8	RET M
	LD	%SP,%HL	/ F9
/FA n n	JP M, nn
	EI		/ FB
/FC n n	CALL M, nn
	ADD	%IY,%BC	/ FD 09
	ADD	%IY,%DE	/ FD 19
/FD21 n n	LD IY, nn
/FD22 n n	LD (nn), IY
	INC	%IY	/ FD 23
	INC	%IYH	/ FD 24
	DEC	%IYH	/ FD 25
/FD26 n 	LD IYH, n*
	ADD	%IY,%IY	/ FD 29
/FD2A n n	LD IY, (nn)
	DEC	%IY	/ FD 2B
	INC	%IYL	/ FD 2C
	DEC	%IYL	/ FD 2D
/FD2E n	LD IYL, n*
/FD34 d	INC (IY + d)
/FD35 d	DEC (IY + d)
/FD36 d n	LD (IY + d), n
	ADD	%IY,%SP	/ FD 39
	LD	%B,%IYH	/ FD 44
	LD	%B,%IYL	/ FD 45
/FD46 d	LD B, (IY + d)
	LD	%C,%IYH	/ FD 4C
	LD	%C,%IYL	/ FD 4D
/FD4E d	LD C, (IY + d)
	LD	%D,%IYH	/ FD 54
	LD	%D,%IYL	/ FD 55
/FD56 d	LD D, (IY + d)
	LD	%E,%IYH	/ FD 5C
	LD	%E,%IYL	/ FD 5D
/FD5E d	LD E, (IY + d)
	LD	%IYH,%B	/ FD 60
	LD	%IYH,%C	/ FD 61
	LD	%IYH,%D	/ FD 62
	LD	%IYH,%E	/ FD 63
	LD	%IYH,%IYH	/ FD 64
	LD	%IYH,%IYL	/ FD 65
/FD66 d	LD H, (IY + d)
	LD	%IYH,%A	/ FD 67
	LD	%IYL,%B	/ FD 68
	LD	%IYL,%C	/ FD 69
	LD	%IYL,%D	/ FD 6A
	LD	%IYL,%E	/ FD 6B
	LD	%IYL,%IYH	/ FD 6C
	LD	%IYL,%IYL	/ FD 6D
/FD6E d	LD L, (IY + d)
	LD	%IYL,%A	/ FD 6F
/FD70 d	LD (IY + d), B
/FD71 d	LD (IY + d), C
/FD72 d	LD (IY + d), D
/FD73 d	LD (IY + d), E
/FD74 d	LD (IY + d), H
/FD75 d	LD (IY + d), L
/FD77 d	LD (IY + d), A
	LD	%A,%IYH	/ FD 7C
	LD	%A,%IYL	/ FD 7D
/FD7E d	LD A, (IY + d)
	ADD	%A,%IYH	/ FD 84
	ADD	%A,%IYL	/ FD 85
/FD86 d	ADD A, (IY + d)
	ADC	%A,%IYH	/ FD 8C
	ADC	%A,%IYL	/ FD 8D
/FD8E d	ADC A, (IY + d)
	SUB	%A,%IYH	/ FD 94
	SUB	%A,%IYL	/ FD 95
/FD96 d	SUB (IY + d)
	SBC	%A,%IYH	/ FD 9C
	SBC	%A,%IYL	/ FD 9D
/FD9E d	SBC A, (IY + d)
	AND	%A,%IYH	/ FD A4
	AND	%A,%IYL	/ FD A5
/FDA6 d	AND (IY + d)
	XOR	%A,%IYH	/ FD AC
	XOR	%A,%IYL	/ FD AD
/FDAE d	XOR (IY + d)
	OR	%A,%IYH	/ FD B4
	OR	%A,%IYL	/ FD B5
/FDB6 d	OR (IY + d)
	CP	%A,%IYH	/ FD BC
	CP	%A,%IYL	/ FD BD
/FDBE d	CP (IY + d)
/FDCB d 00	LD B, RLC (IY + d)*
/FDCB d 01	LD C, RLC (IY + d)*
/FDCB d 02	LD D, RLC (IY + d)*
/FDCB d 03	LD E, RLC (IY + d)*
/FDCB d 04	LD H, RLC (IY + d)*
/FDCB d 05	LD L, RLC (IY + d)*
/FDCB d 06	RLC (IY + d)
/FDCB d 07	LD A, RLC (IY + d)*
/FDCB d 08	LD B, RRC (IY + d)*
/FDCB d 09	LD C, RRC (IY + d)*
/FDCB d 0A	LD D, RRC (IY + d)*
/FDCB d 0B	LD E, RRC (IY + d)*
/FDCB d 0C	LD H, RRC (IY + d)*
/FDCB d 0D	LD L, RRC (IY + d)*
/FDCB d 0E	RRC (IY + d)
/FDCB d 0F	LD A, RRC (IY + d)*
/FDCB d 10	LD B, RL (IY + d)*
/FDCB d 11	LD C, RL (IY + d)*
/FDCB d 12	LD D, RL (IY + d)*
/FDCB d 13	LD E, RL (IY + d)*
/FDCB d 14	LD H, RL (IY + d)*
/FDCB d 15	LD L, RL (IY + d)*
/FDCB d 16	RL (IY + d)
/FDCB d 17	LD A, RL (IY + d)*
/FDCB d 18	LD B, RR (IY + d)*
/FDCB d 19	LD C, RR (IY + d)*
/FDCB d 1A	LD D, RR (IY + d)*
/FDCB d 1B	LD E, RR (IY + d)*
/FDCB d 1C	LD H, RR (IY + d)*
/FDCB d 1D	LD L, RR (IY + d)*
/FDCB d 1E	RR (IY + d)
/FDCB d 1F	LD A, RR (IY + d)*
/FDCB d 20	LD B, SLA (IY + d)*
/FDCB d 21	LD C, SLA (IY + d)*
/FDCB d 22	LD D, SLA (IY + d)*
/FDCB d 23	LD E, SLA (IY + d)*
/FDCB d 24	LD H, SLA (IY + d)*
/FDCB d 25	LD L, SLA (IY + d)*
/FDCB d 26	SLA (IY + d)
/FDCB d 27	LD A, SLA (IY + d)*
/FDCB d 28	LD B, SRA (IY + d)*
/FDCB d 29	LD C, SRA (IY + d)*
/FDCB d 2A	LD D, SRA (IY + d)*
/FDCB d 2B	LD E, SRA (IY + d)*
/FDCB d 2C	LD H, SRA (IY + d)*
/FDCB d 2D	LD L, SRA (IY + d)*
/FDCB d 2E	SRA (IY + d)
/FDCB d 2F	LD A, SRA (IY + d)
/*FDCB d 30	LD B, SLL (IY + d)*
/FDCB d 31	LD C, SLL (IY + d)*
/FDCB d 32	LD D, SLL (IY + d)*
/FDCB d 33	LD E, SLL (IY + d)*
/FDCB d 34	LD H, SLL (IY + d)*
/FDCB d 35	LD L, SLL (IY + d)*
/FDCB d 36	SLL (IY + d)*
/FDCB d 37	LD A, SLL (IY + d)*
/FDCB d 38	LD B, SRL (IY + d)*
/FDCB d 39	LD C, SRL (IY + d)*
/FDCB d 3A	LD D, SRL (IY + d)*
/FDCB d 3B	LD E, SRL (IY + d)*
/FDCB d 3C	LD H, SRL (IY + d)*
/FDCB d 3D	LD L, SRL (IY + d)*
/FDCB d 3E	SRL (IY + d)
/FDCB d 3F	LD A, SRL (IY + d)*
/FDCB d 40	BIT 0, (IY + d)*
/FDCB d 41	BIT 0, (IY + d)*
/FDCB d 42	BIT 0, (IY + d)*
/FDCB d 43	BIT 0, (IY + d)*
/FDCB d 44	BIT 0, (IY + d)*
/FDCB d 45	BIT 0, (IY + d)*
/FDCB d 46	BIT 0, (IY + d)
/FDCB d 47	BIT 0, (IY + d)*
/FDCB d 48	BIT 1, (IY + d)*
/FDCB d 49	BIT 1, (IY + d)*
/FDCB d 4A	BIT 1, (IY + d)*
/FDCB d 4B	BIT 1, (IY + d)*
/FDCB d 4C	BIT 1, (IY + d)*
/FDCB d 4D	BIT 1, (IY + d)*
/FDCB d 4E	BIT 1, (IY + d)
/FDCB d 4F	BIT 1, (IY + d)*
/FDCB d 50	BIT 2, (IY + d)*
/FDCB d 51	BIT 2, (IY + d)*
/FDCB d 52	BIT 2, (IY + d)*
/FDCB d 53	BIT 2, (IY + d)*
/FDCB d 54	BIT 2, (IY + d)*
/FDCB d 55	BIT 2, (IY + d)*
/FDCB d 56	BIT 2, (IY + d)
/FDCB d 57	BIT 2, (IY + d)*
/FDCB d 58	BIT 3, (IY + d)*
/FDCB d 59	BIT 3, (IY + d)*
/FDCB d 5A	BIT 3, (IY + d)*
/FDCB d 5B	BIT 3, (IY + d)*
/FDCB d 5C	BIT 3, (IY + d)*
/FDCB d 5D	BIT 3, (IY + d)*
/FDCB d 5E	BIT 3, (IY + d)
/FDCB d 5F	BIT 3, (IY + d)*
/FDCB d 60	BIT 4, (IY + d)*
/FDCB d 61	BIT 4, (IY + d)*
/FDCB d 62	BIT 4, (IY + d)*
/FDCB d 63	BIT 4, (IY + d)*
/FDCB d 64	BIT 4, (IY + d)*
/FDCB d 65	BIT 4, (IY + d)*
/FDCB d 66	BIT 4, (IY + d)
/FDCB d 67	BIT 4, (IY + d)*
/FDCB d 68	BIT 5, (IY + d)*
/FDCB d 69	BIT 5, (IY + d)*
/FDCB d 6A	BIT 5, (IY + d)*
/FDCB d 6B	BIT 5, (IY + d)*
/FDCB d 6C	BIT 5, (IY + d)*
/FDCB d 6D	BIT 5, (IY + d)*
/FDCB d 6E	BIT 5, (IY + d)
/FDCB d 6F	BIT 5, (IY + d)*
/FDCB d 70	BIT 6, (IY + d)*
/FDCB d 71	BIT 6, (IY + d)*
/FDCB d 72	BIT 6, (IY + d)*
/FDCB d 73	BIT 6, (IY + d)*
/FDCB d 74	BIT 6, (IY + d)*
/FDCB d 75	BIT 6, (IY + d)*
/FDCB d 76	BIT 6, (IY + d)
/FDCB d 77	BIT 6, (IY + d)*
/FDCB d 78	BIT 7, (IY + d)*
/FDCB d 79	BIT 7, (IY + d)*
/FDCB d 7A	BIT 7, (IY + d)*
/FDCB d 7B	BIT 7, (IY + d)*
/FDCB d 7C	BIT 7, (IY + d)*
/FDCB d 7D	BIT 7, (IY + d)*
/FDCB d 7E	BIT 7, (IY + d)
/FDCB d 7F	BIT 7, (IY + d)*
/FDCB d 80	LD B, RES 0, (IY + d)*
/FDCB d 81	LD C, RES 0, (IY + d)*
/FDCB d 82	LD D, RES 0, (IY + d)*
/FDCB d 83	LD E, RES 0, (IY + d)*
/DCB d 84	LD H, RES 0, (IY + d)*
/FDCB d 85	LD L, RES 0, (IY + d)*
/FDCB d 86	RES 0, (IY + d)
/FDCB d 87	LD A, RES 0, (IY + d)*
/FDCB d 88	LD B, RES 1, (IY + d)*
/FDCB d 89	LD C, RES 1, (IY + d)*
/FDCB d 8A	LD D, RES 1, (IY + d)*
/FDCB d 8B	LD E, RES 1, (IY + d)*
/FDCB d 8C	LD H, RES 1, (IY + d)*
/FDCB d 8D	LD L, RES 1, (IY + d)*
/FDCB d 8E	RES 1, (IY + d)
/FDCB d 8F	LD A, RES 1, (IY + d)*
/FDCB d 90	LD B, RES 2, (IY + d)*
/FDCB d 91	LD C, RES 2, (IY + d)*
/FDCB d 92	LD D, RES 2, (IY + d)*
/FDCB d 93	LD E, RES 2, (IY + d)*
/FDCB d 94	LD H, RES 2, (IY + d)*
/FDCB d 95	LD L, RES 2, (IY + d)*
/FDCB d 96	RES 2, (IY + d)
/FDCB d 97	LD A, RES 2, (IY + d)*
/FDCB d 98	LD B, RES 3, (IY + d)*
/FDCB d 99	LD C, RES 3, (IY + d)*
/FDCB d 9A	LD D, RES 3, (IY + d)*
/FDCB d 9B	LD E, RES 3, (IY + d)*
/FDCB d 9C	LD H, RES 3, (IY + d)*
/FDCB d 9D	LD L, RES 3, (IY + d)*
/FDCB d 9E	RES 3, (IY + d)
/FDCB d 9F	LD A, RES 3, (IY + d)*
/FDCB d A0	LD B, RES 4, (IY + d)*
/FDCB d A1	LD C, RES 4, (IY + d)*
/FDCB d A2	LD D, RES 4, (IY + d)*
/FDCB d A3	LD E, RES 4, (IY + d)*
/FDCB d A4	LD H, RES 4, (IY + d)*
/FDCB d A5	LD L, RES 4, (IY + d)*
/FDCB d A6	RES 4, (IY + d)
/FDCB d A7	LD A, RES 4, (IY + d)*
/FDCB d A8	LD B, RES 5, (IY + d)*
/FDCB d A9	LD C, RES 5, (IY + d)*
/FDCB d AA	LD D, RES 5, (IY + d)*
/FDCB d AB	LD E, RES 5, (IY + d)*
/FDCB d AC	LD H, RES 5, (IY + d)*
/FDCB d AD	LD L, RES 5, (IY + d)*
/FDCB d AE	RES 5, (IY + d)
/FDCB d AF	LD A, RES 5, (IY + d)*
/FDCB d B0	LD B, RES 6, (IY + d)*
/FDCB d B1	LD C, RES 6, (IY + d)*
/FDCB d B2	LD D, RES 6, (IY + d)*
/FDCB d B3	LD E, RES 6, (IY + d)*
/FDCB d B4	LD H, RES 6, (IY + d)*
/FDCB d B5	LD L, RES 6, (IY + d)*
/FDCB d B6	RES 6, (IY + d)
/FDCB d B7	LD A, RES 6, (IY + d)*
/FDCB d B8	LD B, RES 7, (IY + d)*
/FDCB d B9	LD C, RES 7, (IY + d)*
/FDCB d BA	LD D, RES 7, (IY + d)*
/FDCB d BB	LD E, RES 7, (IY + d)*
/FDCB d BC	LD H, RES 7, (IY + d)*
/FDCB d BD	LD L, RES 7, (IY + d)*
/FDCB d BE	RES 7, (IY + d)
/FDCB d BF	LD A, RES 7, (IY + d)*
/FDCB d C0	LD B, SET 0, (IY + d)*
/FDCB d C1	LD C, SET 0, (IY + d)*
/FDCB d C2	LD D, SET 0, (IY + d)*
/FDCB d C3	LD E, SET 0, (IY + d)*
/FDCB d C4	LD H, SET 0, (IY + d)*
/FDCB d C5	LD L, SET 0, (IY + d)*
/FDCB d C6	SET 0, (IY + d)
/FDCB d C7	LD A, SET 0, (IY + d)*
/FDCB d C8	LD B, SET 1, (IY + d)*
/FDCB d C9	LD C, SET 1, (IY + d)*
/FDCB d CA	LD D, SET 1, (IY + d)*
/FDCB d CB	LD E, SET 1, (IY + d)*
/FDCB d CC	LD H, SET 1, (IY + d)*
/FDCB d CD	LD L, SET 1, (IY + d)*
/FDCB d CE	SET 1, (IY + d)
/FDCB d CF	LD A, SET 1, (IY + d)*
/FDCB d D0	LD B, SET 2, (IY + d)*
/FDCB d D1	LD C, SET 2, (IY + d)*
/FDCB d D2	LD D, SET 2, (IY + d)*
/FDCB d D3	LD E, SET 2, (IY + d)*
/FDCB d D4	LD H, SET 2, (IY + d)*
/FDCB d D5	LD L, SET 2, (IY + d)*
/FDCB d D6	SET 2, (IY + d)
/FDCB d D7	LD A, SET 2, (IY + d)*
/FDCB d D8	LD B, SET 3, (IY + d)*
/FDCB d D9	LD C, SET 3, (IY + d)*
/FDCB d DA	LD D, SET 3, (IY + d)*
/FDCB d DB	LD E, SET 3, (IY + d)*
/FDCB d DC	LD H, SET 3, (IY + d)*
/FDCB d DD	LD L, SET 3, (IY + d)*
/FDCB d DE	SET 3, (IY + d)
/FDCB d DF	LD A, SET 3, (IY + d)*
/FDCB d E0	LD B, SET 4, (IY + d)*
/FDCB d E1	LD C, SET 4, (IY + d)*
/FDCB d E2	LD D, SET 4, (IY + d)*
/FDCB d E3	LD E, SET 4, (IY + d)*
/FDCB d E4	LD H, SET 4, (IY + d)*
/FDCB d E5	LD L, SET 4, (IY + d)*
/FDCB d E6	SET 4, (IY + d)
/FDCB d E7	LD A, SET 4, (IY + d)*
/FDCB d E8	LD B, SET 5, (IY + d)*
/FDCB d E9	LD C, SET 5, (IY + d)*
/FDCB d EA	LD D, SET 5, (IY + d)*
/FDCB d EB	LD E, SET 5, (IY + d)*
/FDCB d EC	LD H, SET 5, (IY + d)*
/FDCB d ED	LD L, SET 5, (IY + d)*
/FDCB d EE	SET 5, (IY + d)
/FDCB d EF	LD A, SET 5, (IY + d)*
/FDCB d F0	LD B, SET 6, (IY + d)*
/FDCB d F1	LD C, SET 6, (IY + d)*
/FDCB d F2	LD D, SET 6, (IY + d)*
/FDCB d F3	LD E, SET 6, (IY + d)*
/FDCB d F4	LD H, SET 6, (IY + d)*
/FDCB d F5	LD L, SET 6, (IY + d)*
/FDCB d F6	SET 6, (IY + d)
/FDCB d F7	LD A, SET 6, (IY + d)*
/FDCB d F8	LD B, SET 7, (IY + d)*
/FDCB d F9	LD C, SET 7, (IY + d)*
/FDCB d FA	LD D, SET 7, (IY + d)*
/FDCB d FB	LD E, SET 7, (IY + d)*
/FDCB d FC	LD H, SET 7, (IY + d)*
/FDCB d FD	LD L, SET 7, (IY + d)*
/FDCB d FE	SET 7, (IY + d)
/FDCB d FF	LD A, SET 7, (IY + d)*
	POP	%IY	/ FD E1
	EX	(%SP),%IY	/ FD E3
	PUSH	%IY	/ FD E5
/FDE9	JP (IY)
	LD	%SP,%IY	/ FD F9
/FE n	CP n
/FF	RST 38h
