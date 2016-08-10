	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_cr
	.align	4, 0x90
_cr:                                    ## @cr
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp0:
	.cfi_def_cfa_offset 16
	leaq	L_cstrtmp(%rip), %rdi
	callq	_print_string
	popq	%rax
	retq
	.cfi_endproc

	.globl	_displayList
	.align	4, 0x90
_displayList:                           ## @displayList
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%r14
Ltmp1:
	.cfi_def_cfa_offset 16
	pushq	%rbx
Ltmp2:
	.cfi_def_cfa_offset 24
	subq	$24, %rsp
Ltmp3:
	.cfi_def_cfa_offset 48
Ltmp4:
	.cfi_offset %rbx, -24
Ltmp5:
	.cfi_offset %r14, -16
	movl	%edi, 20(%rsp)
	movl	%esi, 16(%rsp)
	movl	$0, 12(%rsp)
	leaq	L_cstrtmp.1(%rip), %rdi
	callq	_print_string
	movl	20(%rsp), %eax
	movl	%eax, 12(%rsp)
	leaq	_list(%rip), %r14
	leaq	L_cstrtmp.2(%rip), %rbx
	jmp	LBB1_1
	.align	4, 0x90
LBB1_6:                                 ## %forassign
                                        ##   in Loop: Header=BB1_1 Depth=1
	incl	12(%rsp)
LBB1_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	movl	20(%rsp), %eax
	addl	16(%rsp), %eax
	cmpl	%eax, 12(%rsp)
	jge	LBB1_7
## BB#2:                                ## %forbody
                                        ##   in Loop: Header=BB1_1 Depth=1
	movslq	12(%rsp), %rax
	movl	(%r14,%rax,4), %edi
	callq	_print_int
	movq	%rbx, %rdi
	callq	_print_string
	movl	12(%rsp), %eax
	subl	20(%rsp), %eax
	incl	%eax
	cltq
	imulq	$1717986919, %rax, %rcx ## imm = 0x66666667
	movq	%rcx, %rdx
	shrq	$63, %rdx
	sarq	$35, %rcx
	addl	%edx, %ecx
	shll	$2, %ecx
	leal	(%rcx,%rcx,4), %ecx
	subl	%ecx, %eax
	addl	$20, %eax
	cltq
	imulq	$1717986919, %rax, %rcx ## imm = 0x66666667
	movq	%rcx, %rdx
	shrq	$63, %rdx
	sarq	$35, %rcx
	addl	%edx, %ecx
	shll	$2, %ecx
	leal	(%rcx,%rcx,4), %ecx
	subl	%ecx, %eax
	sete	%cl
	je	LBB1_4
## BB#3:                                ## %or_right
                                        ##   in Loop: Header=BB1_1 Depth=1
	testl	%eax, %eax
	sete	%al
	movl	12(%rsp), %ecx
	incl	%ecx
	movl	20(%rsp), %edx
	addl	16(%rsp), %edx
	cmpl	%edx, %ecx
	sete	%cl
	orb	%al, %cl
LBB1_4:                                 ## %or_end
                                        ##   in Loop: Header=BB1_1 Depth=1
	testb	%cl, %cl
	je	LBB1_6
## BB#5:                                ## %then
                                        ##   in Loop: Header=BB1_1 Depth=1
	callq	_cr
	jmp	LBB1_6
LBB1_7:                                 ## %endfor
	addq	$24, %rsp
	popq	%rbx
	popq	%r14
	retq
	.cfi_endproc

	.globl	_initList
	.align	4, 0x90
_initList:                              ## @initList
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	movl	$0, -8(%rsp)
	leaq	_list(%rip), %rax
	jmp	LBB2_1
	.align	4, 0x90
LBB2_2:                                 ## %forbody
                                        ##   in Loop: Header=BB2_1 Depth=1
	movslq	-8(%rsp), %rcx
	imull	$2382983, %ecx, %edx    ## imm = 0x245C87
	movslq	%edx, %rdx
	imulq	$1374389535, %rdx, %rsi ## imm = 0x51EB851F
	movq	%rsi, %rdi
	shrq	$63, %rdi
	sarq	$37, %rsi
	addl	%edi, %esi
	imull	$100, %esi, %esi
	subl	%esi, %edx
	addl	$100, %edx
	movslq	%edx, %rdx
	imulq	$1374389535, %rdx, %rsi ## imm = 0x51EB851F
	movq	%rsi, %rdi
	shrq	$63, %rdi
	sarq	$37, %rsi
	addl	%edi, %esi
	imull	$100, %esi, %esi
	subl	%esi, %edx
	movl	%edx, (%rax,%rcx,4)
	incl	-8(%rsp)
LBB2_1:                                 ## %forcond
                                        ## =>This Inner Loop Header: Depth=1
	movl	-8(%rsp), %ecx
	cmpl	-4(%rsp), %ecx
	jl	LBB2_2
## BB#3:                                ## %endfor
	retq
	.cfi_endproc

	.globl	_swap
	.align	4, 0x90
_swap:                                  ## @swap
	.cfi_startproc
## BB#0:                                ## %entry
	movl	%edi, -4(%rsp)
	movl	%esi, -8(%rsp)
	movl	$0, -12(%rsp)
	leaq	_list(%rip), %rax
	movslq	-4(%rsp), %rcx
	movl	(%rax,%rcx,4), %ecx
	movl	%ecx, -12(%rsp)
	movslq	-4(%rsp), %rcx
	movslq	-8(%rsp), %rdx
	movl	(%rax,%rdx,4), %edx
	movl	%edx, (%rax,%rcx,4)
	movslq	-8(%rsp), %rcx
	movl	-12(%rsp), %edx
	movl	%edx, (%rax,%rcx,4)
	retq
	.cfi_endproc

	.globl	_quickSort
	.align	4, 0x90
_quickSort:                             ## @quickSort
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rbp
Ltmp6:
	.cfi_def_cfa_offset 16
Ltmp7:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp8:
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	pushq	%rax
Ltmp9:
	.cfi_offset %rbx, -24
	movl	%edi, -12(%rbp)
	movl	%esi, -16(%rbp)
	subl	-12(%rbp), %esi
	testl	%esi, %esi
	jle	LBB4_2
## BB#1:                                ## %else
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movl	$0, -16(%rax)
	movq	%rsp, %rbx
	leaq	-16(%rbx), %rsp
	movl	$0, -16(%rbx)
	leaq	_list(%rip), %rcx
	movslq	-16(%rbp), %rdx
	movl	(%rcx,%rdx,4), %edx
	movl	%edx, -16(%rax)
	movl	-12(%rbp), %edi
	movl	-16(%rbp), %esi
	callq	_partition
	movl	%eax, -16(%rbx)
	movl	-12(%rbp), %edi
	leal	-1(%rax), %esi
	callq	_quickSort
	movl	-16(%rbx), %edi
	incl	%edi
	movl	-16(%rbp), %esi
	callq	_quickSort
LBB4_2:                                 ## %then
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	retq
	.cfi_endproc

	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rax
Ltmp10:
	.cfi_def_cfa_offset 16
	movl	$0, 4(%rsp)
	movl	$100, 4(%rsp)
	movl	$100, %edi
	callq	_initList
	movl	4(%rsp), %esi
	xorl	%edi, %edi
	callq	_displayList
	movl	4(%rsp), %esi
	decl	%esi
	xorl	%edi, %edi
	callq	_quickSort
	leaq	L_cstrtmp.3(%rip), %rdi
	callq	_print_string
	movl	4(%rsp), %esi
	xorl	%edi, %edi
	callq	_displayList
	popq	%rax
	retq
	.cfi_endproc

	.globl	_partition
	.align	4, 0x90
_partition:                             ## @partition
	.cfi_startproc
## BB#0:                                ## %entry
	pushq	%rbx
Ltmp11:
	.cfi_def_cfa_offset 16
	subq	$32, %rsp
Ltmp12:
	.cfi_def_cfa_offset 48
Ltmp13:
	.cfi_offset %rbx, -16
	movl	%edi, 28(%rsp)
	movl	%esi, 24(%rsp)
	movl	%edx, 20(%rsp)
	movl	$0, 16(%rsp)
	movl	$0, 12(%rsp)
	movl	28(%rsp), %eax
	decl	%eax
	movl	%eax, 16(%rsp)
	movl	24(%rsp), %eax
	movl	%eax, 12(%rsp)
	leaq	_list(%rip), %rbx
	jmp	LBB6_1
LBB6_5:                                 ## %else
                                        ##   in Loop: Header=BB6_1 Depth=1
	movl	16(%rsp), %edi
	movl	12(%rsp), %esi
	callq	_swap
	.align	4, 0x90
LBB6_1:                                 ## %whilebody2
                                        ## =>This Loop Header: Depth=1
                                        ##     Child Loop BB6_2 Depth 2
	incl	16(%rsp)
	movslq	16(%rsp), %rax
	movl	(%rbx,%rax,4), %eax
	cmpl	20(%rsp), %eax
	jl	LBB6_1
	.align	4, 0x90
LBB6_2:                                 ## %whilebody5
                                        ##   Parent Loop BB6_1 Depth=1
                                        ## =>  This Inner Loop Header: Depth=2
	cmpl	$0, 12(%rsp)
	jle	LBB6_4
## BB#3:                                ## %endif7
                                        ##   in Loop: Header=BB6_2 Depth=2
	decl	12(%rsp)
	movslq	12(%rsp), %rax
	movl	(%rbx,%rax,4), %eax
	cmpl	20(%rsp), %eax
	jg	LBB6_2
LBB6_4:                                 ## %endwhile17
                                        ##   in Loop: Header=BB6_1 Depth=1
	movl	16(%rsp), %eax
	cmpl	12(%rsp), %eax
	jl	LBB6_5
## BB#6:                                ## %endwhile25
	movl	16(%rsp), %edi
	movl	24(%rsp), %esi
	callq	_swap
	movl	16(%rsp), %eax
	addq	$32, %rsp
	popq	%rbx
	retq
	.cfi_endproc

	.globl	_list                   ## @list
.zerofill __DATA,__common,_list,400,4
	.section	__TEXT,__cstring,cstring_literals
L_cstrtmp:                              ## @cstrtmp
	.asciz	"\n"

L_cstrtmp.1:                            ## @cstrtmp.1
	.asciz	"List:\n"

L_cstrtmp.2:                            ## @cstrtmp.2
	.asciz	" "

L_cstrtmp.3:                            ## @cstrtmp.3
	.asciz	"After sorting:\n"


.subsections_via_symbols
