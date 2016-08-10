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
	movl	$5, 4(%rsp)
	movl	$0, (%rsp)
	cmpl	$6, 4(%rsp)
	setl	%al
	jl	LBB0_2
## BB#1:                                ## %or_right
	setl	%cl
	movl	4(%rsp), %eax
	cltd
	idivl	(%rsp)
	testl	%eax, %eax
	sete	%al
	orb	%cl, %al
LBB0_2:                                 ## %or_end
	testb	%al, %al
	je	LBB0_4
## BB#3:                                ## %then
	movl	4(%rsp), %edi
	callq	_print_int
LBB0_4:                                 ## %endif
	movl	4(%rsp), %ecx
	cmpl	$5, %ecx
	setl	%al
	cmpl	$4, %ecx
	jg	LBB0_6
## BB#5:                                ## %and_right
	xorl	%eax, %eax
LBB0_6:                                 ## %and_end
	testb	%al, %al
	je	LBB0_8
## BB#7:                                ## %then5
	movl	(%rsp), %edi
	callq	_print_int
LBB0_8:                                 ## %endif7
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc


.subsections_via_symbols
