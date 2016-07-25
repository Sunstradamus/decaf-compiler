	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %or_end
	subq	$24, %rsp
Ltmp0:
	.cfi_def_cfa_offset 32
	movl	$0, 20(%rsp)
	movl	$0, 16(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 11(%rsp)
	movl	$3, 20(%rsp)
	movl	$7, 16(%rsp)
	movb	$1, 11(%rsp)
	movl	$-3, 12(%rsp)
	movl	20(%rsp), %edi
	callq	_print_int
	movl	16(%rsp), %edi
	callq	_print_int
	movzbl	11(%rsp), %edi
	andl	$1, %edi
	callq	_print_int
	movl	12(%rsp), %edi
	callq	_print_int
	xorl	%eax, %eax
	addq	$24, %rsp
	retq
	.cfi_endproc


.subsections_via_symbols
