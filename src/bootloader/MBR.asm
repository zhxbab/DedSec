; ##################################################
; MBR.asm

; function:
; Load boot to memory

; author: Ken
; ##################################################

%include "inc/MBR.inc"


	bits 16


	org BOOT_SEG
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BOOT_SEG
	sti
	cld
	mov si, sp
	mov di, COPY_MBR_SEG
	mov cx, 0x100
	rep movsw
	jmp CONTINUE_MBR_OFF
	times MBR_OFF - ($-$$) db 0
	cmp dl, 0x8f
	jg SKIP_STORE_DL
	mov [HARD_DISK_DRIVE_NUM],dl
SKIP_STORE_DL:
	mov si, BOOT_MESSAGE
	call word SHOW_MESSAGE
; seek active disk range, check 0x1be == 0x80
	mov si, 0x7be
	xor ax, ax
	mov cx, 0x4
CHECK_DISK:
	test byte [si],0x80
	jz NOTACTIVE_DISK
	inc ax
	mov bp, si
NOTACTIVE_DISK:
	add si, 0x10
	loopne CHECK_DISK
	dec ax
	jz FIND_DISK
; int 0x18 boot failure(no active disk range)
	int 0x18
FIND_DISK:
	mov di, 0x5 ;reset disk drive count
SET_LOAD_DISK_INFO:
	mov si, DAP_DATA
	mov word [si+0x2],0x1
	mov eax, [bp+0x8]
	mov [si+0x8], eax
	mov ax, 0x4200
	mov dl, [HARD_DISK_DRIVE_NUM]
	int 0x13
	jnc LOAD_SUCCESS
	dec di
	jz NOT_BOOTABLE_ERROR
	xor ah,ah
	mov dl, [HARD_DISK_DRIVE_NUM]
	int 0x13
	jnz SET_LOAD_DISK_INFO
LOAD_SUCCESS:
	mov ax, [0x7dfe]
	cmp ax, 0xaa55
	jnz NOT_BOOTABLE_ERROR
	mov dl, [HARD_DISK_DRIVE_NUM]
	mov si, DISK_TEMP_ADDRESS
	mov cx, 0x200
	mov di, 0x7c00
	rep movsw
	sti
	jmp 0x00:0x7c00
NOT_BOOTABLE_ERROR:
	mov si, BOOTSTRAP_ERROR
	call SHOW_MESSAGE
	jmp $
SHOW_MESSAGE:
	lodsb
	and al, al
	jz SHOW_MESSAGE_EXIT
	mov ah, 0xe
	mov bx, 0x7
	int 0x10
	jmp SHOW_MESSAGE
SHOW_MESSAGE_EXIT:
	ret


BOOT_MESSAGE:
	db "Start booting from USB device..."
	dw 0x0A0D
	db 0x00
BOOTSTRAP_ERROR:
	db "Not bootable range"
	dw 0x0A0D
	db 0x00
DAP_DATA:
	db 0x10
	db 0x00
	dw 0x0
	dw DISK_TEMP_ADDRESS 	; offset
	dw 0x0					; segment
	dq 0x0
HARD_DISK_DRIVE_NUM:
	db 0x00


	times 446 - ($-$$) db 0

