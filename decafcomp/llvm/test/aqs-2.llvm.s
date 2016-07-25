	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	retq
	.cfi_endproc

	.globl	_list                   ## @list
.zerofill __DATA,__common,_list,400,4

.subsections_via_symbols
