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
	movl	$0, 4(%rsp)
	movl	$0, (%rsp)
	movl	$-14, 4(%rsp)
	movl	$13, (%rsp)
	movl	4(%rsp), %edi
	callq	_print_int
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	movl	(%rsp), %edi
	callq	_print_int
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	movl	4(%rsp), %eax
	cltd
	idivl	(%rsp)
	movl	%eax, %edi
	callq	_print_int
	leaq	L_cstrtmp.2(%rip), %rdi
	callq	_print_string
	movl	4(%rsp), %ecx
	imull	$10000000, %ecx, %eax   ## imm = 0x989680
	movl	(%rsp), %edi
	cltd
	idivl	%edi
	movl	%eax, %esi
	movl	%ecx, %eax
	cltd
	idivl	%edi
	imull	$10000000, %eax, %eax   ## imm = 0x989680
	subl	%eax, %esi
	movl	%esi, %edi
	callq	_abs
	movl	%eax, %edi
	callq	_print_int
	leaq	L_cstrtmp.3(%rip), %rdi
	callq	_print_string
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.globl	_abs
	.align	4, 0x90
_abs:                                   ## @abs
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	testl	%edi, %edi
	jle	LBB1_2
## BB#1:                                ## %then
	movl	-4(%rsp), %eax
	retq
LBB1_2:                                 ## %endif
	xorl	%eax, %eax
	subl	-4(%rsp), %eax
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"/"

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	" = "

L_cstrtmp.2:                            ## @cstrtmp.2
	.asciz	"."

L_cstrtmp.3:                            ## @cstrtmp.3
	.asciz	"\n"


.subsections_via_symbols
