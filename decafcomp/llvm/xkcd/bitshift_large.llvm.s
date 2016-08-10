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
	movl	$2, 16(%rsp)
	movl	20(%rsp), %edi
	movb	16(%rsp), %cl
	shrl	%cl, %edi
	movl	%edi, 12(%rsp)
	shll	$30, %edi
	callq	_print_int
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	movl	12(%rsp), %edi
	shll	$31, %edi
	callq	_print_int
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	xorl	%eax, %eax
	addq	$24, %rsp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"\n"

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	"\n"


.subsections_via_symbols
