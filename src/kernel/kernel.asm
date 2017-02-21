; ##################################################
; kernel.asm

; function:
; DedSec Kernel

; author: Ken
; ##################################################

%include "inc/protect.inc"	; 常量, 宏, 以及一些说明

;import function or global value
extern choose	; int choose(int a, int b);
extern set_gdt  ; void set_gdt()
extern data32_index
extern vedioseg_index
extern protect_main
extern ldt_function
extern ring3main
extern ring0exit
;constant define

code32_index 	equ		0x8
ldt_code_index 	equ		0x4

[section .bss align=16]
StackSpace		resb	4*1024 ; reseverd bytes
StackTop:		; 栈顶
Ring3Stack		resb	1*1024 ; reseverd bytes
Ring3StackTop:		; 栈顶
Ring0Stack		resb	1*1024 ; reseverd bytes
Ring0StackTop:		; 栈顶

[section .data]	; 数据在此
;disp_pos	dd	0

[section .text]	; 代码在此

global _start	; 我们必须导出 _start 这个入口，以便让链接器识别。
;global	disp_str	; 导出这个函数为了让 bar.c 使用
;global disp_pos
global Ring3StackTop
global Ring0StackTop
global ring0_exit

_start:
	cli
	call set_gdt
	mov	ax, [vedioseg_index]
	mov	gs, ax			; 视频段选择子(目的)
	mov ax, [data32_index]
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	jmp code32_index:real_protect_start
real_protect_start:
	mov	esp, StackTop
	call protect_main
	call ldt_code_index:ldt_function
	call ring3main
ring0_exit:
	call ring0exit
	jmp $





