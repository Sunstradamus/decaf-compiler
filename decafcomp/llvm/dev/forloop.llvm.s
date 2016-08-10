	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp0:
	.cfi_def_cfa_offset 16
	movl	$1, %edi
	callq	_print_int
	movl	$2, %edi
	callq	_print_int
	xorl	%edi, %edi
	callq	_print_int
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	movl	$2, %edi
	callq	_print_int
	movl	$1, %edi
	callq	_print_int
	xorl	%edi, %edi
	callq	_print_int
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	movl	$-2, %edi
	callq	_print_int
	movl	$-1, %edi
	callq	_print_int
	xorl	%edi, %edi
	callq	_print_int
	leaq	L_cstrtmp.2(%rip), %rdi
	callq	_print_string
	movl	$-1, %edi
	callq	_print_int
	movl	$-2, %edi
	callq	_print_int
	xorl	%edi, %edi
	callq	_print_int
	leaq	L_cstrtmp.3(%rip), %rdi
	callq	_print_string
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"\n"

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	"\n"

L_cstrtmp.2:                            ## @cstrtmp.2
	.asciz	"\n"

L_cstrtmp.3:                            ## @cstrtmp.3
	.asciz	"\n"


.subsections_via_symbols
