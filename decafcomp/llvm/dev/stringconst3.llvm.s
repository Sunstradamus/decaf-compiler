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
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	leaq	L_cstrtmp.2(%rip), %rdi
	callq	_print_string
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"hello,"

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	" world"

L_cstrtmp.2:                            ## @cstrtmp.2
	.asciz	"\n"


.subsections_via_symbols
