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
code32_index equ 0x8

[SECTION .bss]
StackSpace		resb	256 ; reseverd bytes
StackTop:		; 栈顶

[section .data]	; 数据在此

num1st		dd	3
num2nd		dd	4
disp_pos	dd	0

[section .text]	; 代码在此

global _start	; 我们必须导出 _start 这个入口，以便让链接器识别。
global	disp_str	; 导出这个函数为了让 bar.c 使用


_start:
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
	push	dword [num2nd]	; `.
	push	dword [num1st]	;  |
	call	choose		;  | choose(num1st, num2nd);
	add	esp, 8		; /
	hlt


; ========================================================================
;                  void disp_str(char * info);
; ========================================================================
disp_str:
	push	ebp
	mov	ebp, esp

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [disp_pos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[disp_pos], edi

	pop	ebp
	ret


