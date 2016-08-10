	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	subq	$24, %rsp
Ltmp0:
	.cfi_def_cfa_offset 32
	movl	$0, 20(%rsp)
	movl	$0, 16(%rsp)
	movl	$0, 12(%rsp)
	movl	$4, 20(%rsp)
	movl	$3, 16(%rsp)
	movl	20(%rsp), %edi
	addl	$3, %edi
	movl	%edi, 12(%rsp)
	callq	_print_int
	xorl	%eax, %eax
	addq	$24, %rsp
	retq
	.cfi_endproc


.subsections_via_symbols
