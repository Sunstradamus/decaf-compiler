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
	movl	_a(%rip), %eax
	movl	%eax, 20(%rsp)
	movl	_b(%rip), %esi
	movl	%esi, 16(%rsp)
	movl	20(%rsp), %edi
	callq	_gcd
	movl	%eax, 12(%rsp)
	movl	%eax, %edi
	callq	_print_int
	xorl	%eax, %eax
	addq	$24, %rsp
	retq
	.cfi_endproc

	.globl	_gcd
	.align	4, 0x90
_gcd:                                   ## @gcd
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp1:
	.cfi_def_cfa_offset 16
	movl	%edi, 4(%rsp)
	movl	%esi, (%rsp)
	testl	%esi, %esi
	je	LBB1_1
## BB#2:                                ## %else
	movl	(%rsp), %edi
	movl	4(%rsp), %eax
	cltd
	idivl	%edi
	leal	(%rdx,%rdi), %eax
	cltd
	idivl	%edi
	movl	%edx, %esi
	callq	_gcd
	popq	%rcx
	retq
LBB1_1:                                 ## %then
	movl	4(%rsp), %eax
	popq	%rcx
	retq
	.cfi_endproc

	.section	__DATA,__data
	.globl	_a                      ## @a
	.align	2
_a:
	.long	10                      ## 0xa

	.globl	_b                      ## @b
	.align	2
_b:
	.long	20                      ## 0x14


.subsections_via_symbols
