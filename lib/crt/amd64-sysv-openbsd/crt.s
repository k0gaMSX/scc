	.file	"crt.s"

	.section ".note.openbsd.ident", "a"
	.p2align 2
	.long	8
	.long	4
	.long	1
	.ascii	"OpenBSD\0"
	.long	0
	.p2align 2

	.text
	.align	8
	.global	_start
_start:
	call	main
	movl    %eax, %edi
	call	exit

	.data
	.global	__guard_local
__guard_local:
	.long	0
