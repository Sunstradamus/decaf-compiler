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
	callq	_foo1
	movl	%eax, %edi
	callq	_print_int
	popq	%rax
	retq
	.cfi_endproc

	.globl	_foo1
	.align	4, 0x90
_foo1:                                  ## @foo1
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp1:
	.cfi_def_cfa_offset 16
	callq	_foo2
	incl	%eax
	popq	%rcx
	retq
	.cfi_endproc

	.globl	_foo2
	.align	4, 0x90
_foo2:                                  ## @foo2
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp2:
	.cfi_def_cfa_offset 16
	callq	_foo3
	notb	%al
	movzbl	%al, %edi
	andl	$1, %edi
	callq	_intcast
	popq	%rcx
	retq
	.cfi_endproc

	.globl	_foo3
	.align	4, 0x90
_foo3:                                  ## @foo3
	.cfi_startproc
## BB#0:                                ## %entry
	movb	$1, %al
	retq
	.cfi_endproc

	.globl	_intcast
	.align	4, 0x90
_intcast:                               ## @intcast
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	movl	%edi, %eax
	retq
	.cfi_endproc


.subsections_via_symbols
