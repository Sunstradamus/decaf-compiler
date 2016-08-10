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
	callq	_init_array
	callq	_print_array
	xorl	%edi, %edi
	movl	$1, %esi
	callq	_sort_array
	callq	_print_array
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.globl	_init_array
	.align	4, 0x90
_init_array:                            ## @init_array
	.cfi_startproc
## BB#0:                                ## %entry
	movl	$0, -4(%rsp)
	leaq	_a(%rip), %rax
	jmp	LBB1_1
	.align	4, 0x90
LBB1_2:                                 ## %forbody
                                        ##   in Loop: Header=BB1_1 Depth=1
	movslq	-4(%rsp), %rcx
	movl	%ecx, %edx
	negl	%edx
	imull	$23, %edx, %edx
	movslq	%edx, %rdx
	imulq	$1321528399, %rdx, %rsi ## imm = 0x4EC4EC4F
	movq	%rsi, %rdi
	shrq	$63, %rdi
	sarq	$34, %rsi
	addl	%edi, %esi
	imull	$13, %esi, %esi
	subl	%esi, %edx
	addl	$13, %edx
	movslq	%edx, %rdx
	imulq	$1321528399, %rdx, %rsi ## imm = 0x4EC4EC4F
	movq	%rsi, %rdi
	shrq	$63, %rdi
	sarq	$34, %rsi
	addl	%edi, %esi
	imull	$13, %esi, %esi
	subl	%esi, %edx
	movl	%edx, (%rax,%rcx,4)
	incl	-4(%rsp)
LBB1_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	movl	-4(%rsp), %ecx
	cmpl	_size(%rip), %ecx
	jl	LBB1_2
## BB#3:                                ## %endfor
	retq
	.cfi_endproc

	.globl	_sort_array
	.align	4, 0x90
_sort_array:                            ## @sort_array
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rbp
Ltmp1:
	.cfi_def_cfa_offset 16
	pushq	%r15
Ltmp2:
	.cfi_def_cfa_offset 24
	pushq	%r14
Ltmp3:
	.cfi_def_cfa_offset 32
	pushq	%rbx
Ltmp4:
	.cfi_def_cfa_offset 40
	subq	$24, %rsp
Ltmp5:
	.cfi_def_cfa_offset 64
Ltmp6:
	.cfi_offset %rbx, -40
Ltmp7:
	.cfi_offset %r14, -32
Ltmp8:
	.cfi_offset %r15, -24
Ltmp9:
	.cfi_offset %rbp, -16
	movl	%edi, 20(%rsp)
	movl	%esi, 16(%rsp)
	movl	$0, 12(%rsp)
	leaq	_a(%rip), %r14
	jmp	LBB2_1
	.align	4, 0x90
LBB2_10:                                ## %forassign
                                        ##   in Loop: Header=BB2_1 Depth=1
	movslq	20(%rsp), %rax
	movl	(%r14,%rax,4), %eax
	movl	%eax, 12(%rsp)
	movslq	20(%rsp), %rax
	movslq	16(%rsp), %rcx
	movl	(%r14,%rcx,4), %ecx
	movl	%ecx, (%r14,%rax,4)
	movslq	16(%rsp), %rax
	movl	12(%rsp), %ecx
	movl	%ecx, (%r14,%rax,4)
LBB2_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	movslq	20(%rsp), %rax
	movl	(%r14,%rax,4), %r15d
	movslq	16(%rsp), %rax
	movl	(%r14,%rax,4), %ebp
	cmpl	%ebp, %r15d
	setg	%bl
	jg	LBB2_5
## BB#2:                                ## %or_right
                                        ##   in Loop: Header=BB2_1 Depth=1
	movl	_size(%rip), %eax
	decl	%eax
	cmpl	%eax, 16(%rsp)
	setl	%al
	jge	LBB2_4
## BB#3:                                ## %and_right
                                        ##   in Loop: Header=BB2_1 Depth=1
	setl	%bl
	movl	20(%rsp), %edi
	movl	16(%rsp), %esi
	incl	%esi
	callq	_sort_array
	andb	%bl, %al
LBB2_4:                                 ## %and_end
                                        ##   in Loop: Header=BB2_1 Depth=1
	cmpl	%ebp, %r15d
	setg	%bl
	orb	%al, %bl
LBB2_5:                                 ## %or_end
                                        ##   in Loop: Header=BB2_1 Depth=1
	testb	%bl, %bl
	jne	LBB2_9
## BB#6:                                ## %or_right6
                                        ##   in Loop: Header=BB2_1 Depth=1
	movl	_size(%rip), %eax
	addl	$-2, %eax
	cmpl	%eax, 20(%rsp)
	setl	%al
	jge	LBB2_8
## BB#7:                                ## %and_right11
                                        ##   in Loop: Header=BB2_1 Depth=1
	setl	%bpl
	movl	20(%rsp), %esi
	leal	1(%rsi), %edi
	addl	$2, %esi
	callq	_sort_array
	andb	%bpl, %al
LBB2_8:                                 ## %and_end18
                                        ##   in Loop: Header=BB2_1 Depth=1
	orb	%al, %bl
LBB2_9:                                 ## %or_end21
                                        ##   in Loop: Header=BB2_1 Depth=1
	testb	%bl, %bl
	jne	LBB2_10
## BB#11:                               ## %endfor
	xorl	%eax, %eax
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_print_array
	.align	4, 0x90
_print_array:                           ## @print_array
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp10:
	.cfi_def_cfa_offset 16
	movl	$0, 4(%rsp)
	movl	$0, (%rsp)
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	movl	$0, 4(%rsp)
	movl	$0, (%rsp)
	jmp	LBB3_1
	.align	4, 0x90
LBB3_2:                                 ## %forassign
                                        ##   in Loop: Header=BB3_1 Depth=1
	movl	(%rsp), %edi
	callq	_print_element
	movl	%eax, 4(%rsp)
	incl	(%rsp)
LBB3_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	movl	_size(%rip), %eax
	decl	%eax
	cmpl	%eax, (%rsp)
	jl	LBB3_2
## BB#3:                                ## %endfor
	movslq	(%rsp), %rax
	leaq	_a(%rip), %rcx
	movl	(%rcx,%rax,4), %edi
	callq	_print_int
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	popq	%rax
	retq
	.cfi_endproc

	.globl	_print_element
	.align	4, 0x90
_print_element:                         ## @print_element
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp11:
	.cfi_def_cfa_offset 16
	movl	%edi, 4(%rsp)
	leaq	_a(%rip), %rax
	movslq	4(%rsp), %rcx
	movl	(%rax,%rcx,4), %edi
	callq	_print_int
	leaq	L_cstrtmp.2(%rip), %rdi
	callq	_print_string
	xorl	%eax, %eax
	popq	%rcx
	retq
	.cfi_endproc

	.section	__DATA,__data
	.globl	_size                   ## @size
	.align	2
_size:
	.long	13                      ## 0xd

	.globl	_a                      ## @a
.zerofill __DATA,__common,_a,52,4
	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"a=["

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	"]\n"

L_cstrtmp.2:                            ## @cstrtmp.2
	.asciz	","


.subsections_via_symbols
