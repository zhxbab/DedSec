; ##################################################
; boot.asm
;
; function: Load protect, page, long, sectors
;
; author: Ken
; ##################################################

%include "inc/MBR.inc"
%include "inc/boot.inc"

	bits 16


	org 0x7c00
Start:
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov [HARD_DISK_DRIVE_NUM], dl
	call ResetScreen
;	mov ax, BootMessage
;	call DispStr
	mov eax, SETUP_SINDEX
	mov di, SETUP_SADDR
	mov bx, SETUP_NUM
	call Read_Sector
	mov eax, KERNEL_SINDEX
	mov di, KERNEL_SADDR
	mov bx, KERNEL_NUM
	call Read_Sector
	jmp SETUP_SADDR
	;jmp $
	; loop forever

DispStr: ; ax indicate Message
	mov bp, ax
	; ES:BP string address
	mov cx, 16
	; CX string size
	mov ax, 0x1301
	; AH = 13h, AL = 01h
	mov bx, 0x7
	; BH = 0, BL = 0x7, hignlight. white word and black backgroud
	mov dl, 0
	int 0x10
	ret

ResetScreen: ; reset playmode
	mov ah, 0x0
	mov al, 0x3
	int 0x10
	ret

DapStruct:
	db 0x10
	db 0x00
	dw 0x0
	dw DISK_TEMP_ADDRESS 	; offset
	dw 0x0					; segment
	dq 0x0


Read_Sector: ;
;###########input reg################
;eax: sector index
;di: sector addr
;bx: sector num
;####################################
	mov si, DapStruct
	mov word [si+0x2],bx
	mov [si+0x8], eax
	mov ax, 0x4200
	mov dl, [HARD_DISK_DRIVE_NUM]
	int 0x13
	jnc sector_load_success
	mov ax, SectorLoadFailMessage
	call DispStr
	ret
sector_load_success:
	mov si, DISK_TEMP_ADDRESS
	mov cx, 0x0
set_size:
	add cx, 0x200
	dec bx
	jnz set_size
	rep movsw
	ret




BootMessage:
	db "Hello DedSec!"
	dw 0x0A0D
	db 0x00
SectorLoadFailMessage:
	db "Load Sector Fail!"
	dw 0x0A0D
	db 0x00
HARD_DISK_DRIVE_NUM:
	db 0x00

	times 510 - ($-$$) db 0
	dw 0xaa55
