	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	movl	$2, -4(%rbp)
	xorl	%eax, %eax
	testb	%al, %al
	jne	LBB0_2
## BB#1:                                ## %then
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movb	$0, -16(%rax)
	movb	$1, -16(%rax)
LBB0_2:                                 ## %endif
	movl	-4(%rbp), %edi
	callq	_print_int
	xorl	%eax, %eax
	movq	%rbp, %rsp
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
