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
	jmp	LBB0_1
	.align	4, 0x90
LBB0_9:                                 ## %forassign
                                        ##   in Loop: Header=BB0_1 Depth=1
	incl	4(%rsp)
LBB0_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	cmpl	$19, 4(%rsp)
	jg	LBB0_8
## BB#2:                                ## %forbody
                                        ##   in Loop: Header=BB0_1 Depth=1
	cmpl	$2, 4(%rsp)
	jl	LBB0_9
## BB#3:                                ## %then
                                        ##   in Loop: Header=BB0_1 Depth=1
	cmpl	$3, 4(%rsp)
	jl	LBB0_9
## BB#4:                                ## %then4
                                        ##   in Loop: Header=BB0_1 Depth=1
	cmpl	$4, 4(%rsp)
	jl	LBB0_9
## BB#5:                                ## %then7
                                        ##   in Loop: Header=BB0_1 Depth=1
	cmpl	$10, 4(%rsp)
	jl	LBB0_9
## BB#6:                                ## %else
                                        ##   in Loop: Header=BB0_1 Depth=1
	cmpl	$14, 4(%rsp)
	jg	LBB0_9
## BB#7:                                ## %endwhile
	movl	4(%rsp), %edi
	callq	_print_int
LBB0_8:                                 ## %endfor
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc


.subsections_via_symbols
