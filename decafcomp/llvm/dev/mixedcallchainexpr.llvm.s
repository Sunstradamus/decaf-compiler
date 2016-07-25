	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_test3
	.align	4, 0x90
_test3:                                 ## @test3
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp0:
	.cfi_def_cfa_offset 16
	andl	$1, %edi
	movb	%dil, 7(%rsp)
	movl	%esi, (%rsp)
	movl	%esi, %edi
	callq	_print_int
	movzbl	7(%rsp), %edi
	andl	$1, %edi
	callq	_print_int
	popq	%rax
	retq
	.cfi_endproc

	.globl	_test2
	.align	4, 0x90
_test2:                                 ## @test2
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp1:
	.cfi_def_cfa_offset 16
	movl	%edi, 4(%rsp)
	andl	$1, %esi
	movb	%sil, 3(%rsp)
	movl	4(%rsp), %edi
	callq	_print_int
	movzbl	3(%rsp), %edi
	andl	$1, %edi
	callq	_print_int
	movb	3(%rsp), %al
	xorb	$1, %al
	movl	4(%rsp), %esi
	incl	%esi
	movzbl	%al, %edi
	callq	_test3
	popq	%rax
	retq
	.cfi_endproc

	.globl	_test1
	.align	4, 0x90
_test1:                                 ## @test1
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp2:
	.cfi_def_cfa_offset 16
	andl	$1, %edi
	movb	%dil, 7(%rsp)
	movl	%esi, (%rsp)
	movl	%esi, %edi
	callq	_print_int
	movzbl	7(%rsp), %edi
	andl	$1, %edi
	callq	_print_int
	movl	(%rsp), %edi
	incl	%edi
	movb	7(%rsp), %al
	xorb	$1, %al
	movzbl	%al, %esi
	callq	_test2
	popq	%rax
	retq
	.cfi_endproc

	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp3:
	.cfi_def_cfa_offset 16
	movl	$1, %edi
	movl	$1, %esi
	callq	_test1
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc


.subsections_via_symbols
