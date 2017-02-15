/* ##################################################
; protect.c

; function:
; config protect mode

; author: Ken
; ##################################################
*/

#include "../lib/inc/const.h"
#include "inc/protect.h"


DESCRIPTOR32 gdt_des[4] = {
		[0].data = 0x0,
		gdt_des_bitmap(1,0xffff,0x0000,0x00,0xA,1,0,1,0xf,1,0,1,1,0x00),
		gdt_des_bitmap(2,0xffff,0x0000,0x00,0x2,1,0,1,0xf,1,0,1,1,0x00),
		gdt_des_bitmap(3,0xffff,0x8000,0x0B,0x2,1,0,1,0x0,1,0,0,0,0x00),
};

GDTR32 gdtr = {
		.limit = GDT_SIZE,
		.base = (unsigned int)gdt_des
};

unsigned code32_index = 0x8;
unsigned data32_index = 0x10;
unsigned vedioseg_index = 0x18;

void set_gdt(){

	__asm__ __volatile__("lgdt %0" : :"m"(gdtr));

}

