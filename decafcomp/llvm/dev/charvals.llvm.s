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
	movl	$97, %edi
	callq	_print_int
	movl	$109, %edi
	callq	_print_int
	movl	$122, %edi
	callq	_print_int
	movl	$65, %edi
	callq	_print_int
	movl	$77, %edi
	callq	_print_int
	movl	$90, %edi
	callq	_print_int
	movl	$48, %edi
	callq	_print_int
	movl	$55, %edi
	callq	_print_int
	movl	$57, %edi
	callq	_print_int
	movl	$45, %edi
	callq	_print_int
	movl	$125, %edi
	callq	_print_int
	movl	$34, %edi
	callq	_print_int
	movl	$94, %edi
	callq	_print_int
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc


.subsections_via_symbols
