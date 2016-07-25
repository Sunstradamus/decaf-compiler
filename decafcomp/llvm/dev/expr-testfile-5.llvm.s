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
	movb	$1, 7(%rsp)
	movb	$1, 6(%rsp)
	movb	7(%rsp), %al
	andb	$1, %al
	movzbl	%al, %ecx
	cmpl	$1, %ecx
	jne	LBB0_2
## BB#1:                                ## %and_right
	andb	6(%rsp), %al
LBB0_2:                                 ## %and_end
	movb	%al, 5(%rsp)
	movzbl	5(%rsp), %edi
	andl	$1, %edi
	callq	_print_int
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc


.subsections_via_symbols
