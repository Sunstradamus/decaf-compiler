	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_f
	.align	4, 0x90
_f:                                     ## @f
	.cfi_startproc
## BB#0:                                ## %entry
	.align	4, 0x90
LBB0_1:                                 ## %whilecond
                                        ## =>This Inner Loop Header: Depth=1
	jmp	LBB0_1
	.cfi_endproc


.subsections_via_symbols
