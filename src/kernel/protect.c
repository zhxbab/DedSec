/* ##################################################
; protect.c

; function:
; config protect mode

; author: Ken
; ##################################################
*/

#include "inc/protect.h"
#include "../lib/inc/klib.h"
#include "../lib/inc/stdlib.h"
#include "inc/int.h"
#include "../lib/inc/mem.h"


//#define call_gate_ring3 0x2a

extern u32 Ring3StackTop;
extern u32 Ring0StackTop;
extern void ring0_exit();

DESCRIPTOR32 gdt_des[16] = {
		init_descriptor32_data(GDT_RESERVERD,0x0,0x0),
		init_descriptor32_bitmap(GDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel Code Seg
		init_descriptor32_bitmap(GDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel Data Seg
		init_descriptor32_bitmap(GDT_VRAM,0xffff,0x8000,0x0B,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY,0x00), // VRAM Seg
		init_descriptor32_bitmap(GDT_LDT_DESCRIPTOR,0xffff,0x0000,0x00,DA_LDT,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel LDT
		init_gate32_data(GDT_CALL_GATE32,0x0,0x0),// ring0 call gate
		init_descriptor32_bitmap(GDT_RING3_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Ring3 Code Seg
		init_descriptor32_bitmap(GDT_RING3_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Ring3 Data Seg
		init_descriptor32_bitmap(GDT_TSS32_DESCRIPTOR,0x0,0x0,0x0,DA_386TSS,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_INVALID,SEG_GRANULARITY,0x00), // available Tss32 Descriptor
};

GDTR32 gdtr = {
		.limit = GDT_SIZE,
		.base = (unsigned int)gdt_des
};


DESCRIPTOR32 ldt_des[3] = {
		init_descriptor32_bitmap(LDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT Code Seg
		init_descriptor32_bitmap(LDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT Data Seg
};

TSS32 tss32;
TR tr;

unsigned short code32_index = 0x8;
unsigned short code32_index_ring3 = 0x33;
unsigned short data32_index_ring3 = 0x3b;
unsigned short data32_index = 0x10;
unsigned short vedioseg_index = 0x18;
unsigned short ldt_index = 0x20;
unsigned short ldt_code_index = 0x4;
unsigned short ldt_data_index = 0xc;
unsigned short call_gate_ring3 = 0x2a;
unsigned short tss32_index = 0x43;

void set_gdt(){
	__asm__ __volatile__("lgdt %0" : :"m"(gdtr));
}


void init_interrput(){
	disp_str("Initial Interrupt\n");
	init_8259A();
	set_idt();
}

void init_ldt(){
	set_ldt_descriptor(&gdt_des[LDT_DES_INDEX],0,(u32)ldt_des,0x20);
	__asm__ __volatile__("lldt %0"::"m"(ldt_index));
}

void init_call_gate32(){
	disp_str("Init Call gate\n");
	set_call_gate32(&gdt_des[CALL_GATE_R3_INDEX],3,(u32)ring0_exit,code32_index);

}

void set_ldt_descriptor(DESCRIPTOR32 *pdescripot, u8 dpl, u32 base, u32 limit){
	pdescripot->bitmap.seg_limit_0 = limit&0xFFFF;
	pdescripot->bitmap.seg_limit_1 = (limit>>16)&0xFFFF;
	pdescripot->bitmap.base_0 = base&0xFFFF;
	pdescripot->bitmap.base_1 = (base>>16)&0xFF;
	pdescripot->bitmap.base_2 = (base>>24)&0xFF;
	pdescripot->bitmap.dpl = dpl;
}

void set_call_gate32(DESCRIPTOR32 *pdescripot, u8 dpl, u32 offset, u16 selector){
	pdescripot->call_gate32_bitmap_des.seg_s = selector;
	pdescripot->call_gate32_bitmap_des.offset_0 = offset&0xFFFF;
	pdescripot->call_gate32_bitmap_des.offset_1 = (offset>>16)&0xFF;
	pdescripot->call_gate32_bitmap_des.dpl = dpl;
}

void protect_main(){
	disp_str("Enter Protect Main\n");
	init_interrput();
	init_ldt();
	init_call_gate32();
	init_tss32();
//	__asm__ __volatile__("int $0x80");
}

void init_tss32(){
	set_tss32(&tss32);
	gdt_des[TSS_DES_INDEX].tss32_bitmap_des.base_0 = (u16)(((u32)&tss32)&0xFFFF);
	gdt_des[TSS_DES_INDEX].tss32_bitmap_des.base_1 = (u16)((((u32)&tss32)>>16)&0xFF);
	gdt_des[TSS_DES_INDEX].tss32_bitmap_des.base_2 = (u16)((((u32)&tss32)>>24)&0xFF);
	gdt_des[TSS_DES_INDEX].tss32_bitmap_des.seg_limit_0 = sizeof(tss32)&0xffff;
	gdt_des[TSS_DES_INDEX].tss32_bitmap_des.seg_limit_1 = (sizeof(tss32)>>16)&0xff;
	__asm__ __volatile__("ltr %0"::"m"(tss32_index));
}

void ldt_function(){
	disp_str("Enter LDT function\n");
}

void ring0exit(){
	disp_str("Enter Ring0 again\n");
}
void ring3start(){
	disp_str("Enter Ring3\n");
	ICALL32 icall;
	icall.offset = 0;
	icall.seg = call_gate_ring3;
	__asm__ __volatile__("lcall *%0"::"m"(icall));
}

void set_tss32(TSS32* ptss32){
	ptss32->ss0 = (u32)data32_index;
	ptss32->esp0 = (u32)&Ring0StackTop;
	ptss32->iobase = (u16)((u32)ptss32->iomap-(u32)ptss32);
	memset(ptss32->iomap,0,127);
	ptss32->iomap[127] = 0xff;

}
void ring3main(){
	STACK_INFO32 stack_info;
	stack_info.ss = data32_index_ring3;
	stack_info.esp = (u32)&Ring3StackTop;
	stack_info.cs = code32_index_ring3;
	stack_info.eip = (u32)ring3start;
	__asm__ __volatile__("movw %0, %%ax"::"m"(stack_info.ss));
	__asm__ __volatile__("movw %ax, %ds");
	__asm__ __volatile__("pushl %0"::"m"(stack_info.ss)); //stack swith need every entry 4-bytes align
	__asm__ __volatile__("pushl %0"::"m"(stack_info.esp));
	__asm__ __volatile__("pushl %0"::"m"(stack_info.cs));
	__asm__ __volatile__("pushl %0"::"m"(stack_info.eip));
	__asm__ __volatile__(".byte 0xCB");
}

