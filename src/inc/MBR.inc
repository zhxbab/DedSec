; BOOT_SEG is MBR sector
; COPY_MBR_SEG is copied MBR sector
; CONTINUE_MBR_OFF is jump address for completing copy MBR
; DISK_TEMP_ADDRESS store the data which int0x13 return

BOOT_SEG: 				equ		0x7c00
COPY_MBR_SEG:			equ 	0x600
CONTINUE_MBR_OFF:		equ 	0x6a0
MBR_OFF: 				equ		0xa0
DISK_TEMP_ADDRESS: 		equ 	0x800