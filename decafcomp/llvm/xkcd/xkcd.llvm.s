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
	callq	_d
	movl	%eax, %edi
	callq	_c
	movl	%eax, %edi
	callq	_k
	movl	%eax, %edi
	callq	_x
	movl	%eax, %edi
	callq	_print_int
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.globl	_x
	.align	4, 0x90
_x:                                     ## @x
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	leal	(%rdi,%rdi), %eax
	retq
	.cfi_endproc

	.globl	_k
	.align	4, 0x90
_k:                                     ## @k
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	leal	(%rdi,%rdi,2), %eax
	retq
	.cfi_endproc

	.globl	_c
	.align	4, 0x90
_c:                                     ## @c
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	leal	4(%rdi), %eax
	retq
	.cfi_endproc

	.globl	_d
	.align	4, 0x90
_d:                                     ## @d
	.cfi_startproc
## BB#0:                                ## %entry
	movl	$3, %eax
	retq
	.cfi_endproc


.subsections_via_symbols
