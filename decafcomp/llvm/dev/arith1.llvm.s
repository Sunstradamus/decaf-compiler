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
	movb	$0, 7(%rsp)
	movb	$0, 6(%rsp)
	movb	$0, 5(%rsp)
	movb	$0, 4(%rsp)
	movl	$0, (%rsp)
	movb	$1, 6(%rsp)
	movb	$0, 5(%rsp)
	movb	$1, 4(%rsp)
	movb	6(%rsp), %al
	andb	$1, %al
	jne	LBB0_4
## BB#1:                                ## %or_right
	movb	5(%rsp), %cl
	andb	$1, %cl
	movzbl	%cl, %edx
	cmpl	$1, %edx
	jne	LBB0_3
## BB#2:                                ## %and_right
	movb	4(%rsp), %dl
	xorb	$1, %dl
	andb	%dl, %cl
LBB0_3:                                 ## %and_end
	orb	%cl, %al
LBB0_4:                                 ## %or_end
	movb	%al, 7(%rsp)
	movl	$0, (%rsp)
	xorl	%edi, %edi
	callq	_print_int
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc


.subsections_via_symbols
