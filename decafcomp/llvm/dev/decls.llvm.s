	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_f
	.align	4, 0x90
_f:                                     ## @f
	.cfi_startproc
## BB#0:                                ## %entry
	movl	$0, -4(%rsp)
	movl	$0, -8(%rsp)
	movl	$100, _size(%rip)
	retq
	.cfi_endproc

	.globl	_x                      ## @x
.zerofill __DATA,__common,_x,4,2
	.globl	_size                   ## @size
.zerofill __DATA,__common,_size,4,2

.subsections_via_symbols
