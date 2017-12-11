	.file	"crt.s"

	.text
	.global	_start
_start:
	call	main
#	ex	%de,%hl
	call	exit
