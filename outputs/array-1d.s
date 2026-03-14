	.build_version macos, 26, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	str	wzr, [sp, #8]
	b	LBB0_1
LBB0_1:                                 ; %loop.cond
                                        ; =>This Inner Loop Header: Depth=1
	ldr	w8, [sp, #8]
	subs	w8, w8, #10
	cset	w8, lt
	cbz	w8, LBB0_3
	b	LBB0_2
LBB0_2:                                 ; %loop.body
                                        ;   in Loop: Header=BB0_1 Depth=1
	ldrsw	x9, [sp, #8]
	adrp	x8, _Array1D.global@PAGE
	add	x8, x8, _Array1D.global@PAGEOFF
	ldr	w8, [x8, x9, lsl #2]
	str	w8, [sp, #12]
	ldr	w8, [sp, #8]
	add	w8, w8, #1
	str	w8, [sp, #8]
	b	LBB0_1
LBB0_3:                                 ; %loop.end
	str	wzr, [sp, #8]
	b	LBB0_4
LBB0_4:                                 ; %loop.cond6
                                        ; =>This Inner Loop Header: Depth=1
	ldr	w8, [sp, #8]
	subs	w8, w8, #10
	cset	w8, lt
	cbz	w8, LBB0_6
	b	LBB0_5
LBB0_5:                                 ; %loop.body7
                                        ;   in Loop: Header=BB0_4 Depth=1
	ldr	w8, [sp, #12]
                                        ; kill: def $x8 killed $w8
	mov	x9, sp
	str	x8, [x9]
	adrp	x0, l_printf.num@PAGE
	add	x0, x0, l_printf.num@PAGEOFF
	bl	_printf
	ldr	w8, [sp, #8]
	add	w8, w8, #1
	str	w8, [sp, #8]
	b	LBB0_4
LBB0_6:                                 ; %loop.end8
	mov	w0, #0                          ; =0x0
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_Array1D.global                 ; @Array1D.global
.zerofill __DATA,__common,_Array1D.global,1394092160,4
	.section	__TEXT,__cstring,cstring_literals
l___unnamed_1:                          ; @0
	.asciz	"\n"

l_printf.num:                           ; @printf.num
	.asciz	"%d\n"

.subsections_via_symbols
