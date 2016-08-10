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
	movl	$3, 4(%rsp)
	movl	$5, (%rsp)
	movl	$6, _a(%rip)
	movl	$5, _a+4(%rip)
	movl	$7, 4(%rsp)
	jmp	LBB0_1
	.align	4, 0x90
LBB0_4:                                 ## %forbody
                                        ##   in Loop: Header=BB0_1 Depth=1
	incl	4(%rsp)
LBB0_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	movl	_a+4(%rip), %eax
	cmpl	%eax, _a(%rip)
	setl	%al
	jl	LBB0_3
## BB#2:                                ## %or_right
                                        ##   in Loop: Header=BB0_1 Depth=1
	setl	%cl
	cmpl	$10, 4(%rsp)
	setl	%al
	orb	%cl, %al
LBB0_3:                                 ## %or_end
                                        ##   in Loop: Header=BB0_1 Depth=1
	testb	%al, %al
	jne	LBB0_4
## BB#5:                                ## %endfor
	movl	4(%rsp), %edi
	callq	_print_int
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.globl	_a                      ## @a
.zerofill __DATA,__common,_a,40,4

.subsections_via_symbols
