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
	movb	16(%rsp), %cl
	shll	%cl, %edi
	movl	%edi, 12(%rsp)
	shrl	$2, %edi
	callq	_print_int
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	movl	12(%rsp), %edi
	shrl	$5, %edi
	callq	_print_int
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	movl	12(%rsp), %edi
	shrl	$6, %edi
	callq	_print_int
	leaq	L_cstrtmp.2(%rip), %rdi
	callq	_print_string
	movb	$1, %al
	addq	$24, %rsp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"\n"

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	"\n"

L_cstrtmp.2:                            ## @cstrtmp.2
	.asciz	"\n"


.subsections_via_symbols
