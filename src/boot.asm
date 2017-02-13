; ##################################################
; boot.asm
;
;
; author: Ken
; ##################################################
	bits 16
	org 0x7c00
start:
	mov ax, cs
	mov ds, ax
	mov es, ax
	call ResetScreen
	call DispStr
	jmp $
	; loop forever
DispStr:
	mov ax, BootMessage
	mov bp, ax
	; ES:BP string address
	mov cx, 16
	; CX string size
	mov ax, 0x1301
	; AH = 13h, AL = 01h
	mov bx, 0xc
	; BH = 0, BL = 0xc, hignlight. red word and black backgroud
	mov dl, 0
	int 0x10
	ret
ResetScreen: ; reset playmode
	mov ah, 0x0
	mov al, 0x3
	int 0x10
	ret

BootMessage:
	db "Hello DedSec!"

;times 446 - ($-$$) db 0
	times 510 - ($-$$) db 0
	dw 0xaa55
