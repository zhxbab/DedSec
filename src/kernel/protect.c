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

volatile DESCRIPTOR32 gdt_des[16] = {
		init_descriptor32_data(GDT_RESERVERD,0x0,0x0),
		init_descriptor32_bitmap(GDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel Code Seg
		init_descriptor32_bitmap(GDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel Data Seg
		init_descriptor32_bitmap(GDT_VRAM,0xffff,0x8000,0x0B,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_1,0x00), // VRAM Seg
		init_descriptor32_bitmap(GDT_LDT_DESCRIPTOR,0xffff,0x0000,0x00,DA_LDT,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel LDT
		init_gate32_data(GDT_CALL_GATE32,0x0,0x0),// ring0 call gate
		init_descriptor32_bitmap(GDT_RING3_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Ring3 Code Seg
		init_descriptor32_bitmap(GDT_RING3_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Ring3 Data Seg
		init_descriptor32_bitmap(GDT_TSS32_DESCRIPTOR,0x0,0x0,0x0,DA_386TSS,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_INVALID,SEG_GRANULARITY_1,0x00), // available Tss32 Descriptor
};

volatile GDTR32 gdtr = {
		.limit = GDT_SIZE,
		.base = (unsigned int)gdt_des
};


volatile DESCRIPTOR32 ldt_des[2] = {
		init_descriptor32_bitmap(LDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT Code Seg
		init_descriptor32_bitmap(LDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT Data Seg
};

TSS32 tss32;
volatile TR tr;

volatile unsigned short sel_kernel_code = set_gdt_selector(GDT_KERNEL_CODE,RPL_PRIVILEGE_KRNL);
volatile unsigned short sel_kernel_data = set_gdt_selector(GDT_KERNEL_DATA,RPL_PRIVILEGE_KRNL);
volatile unsigned short sel_vram = set_gdt_selector(GDT_VRAM,RPL_PRIVILEGE_USER);
volatile unsigned short sel_ldt_des = set_gdt_selector(GDT_LDT_DESCRIPTOR,RPL_PRIVILEGE_KRNL);
volatile unsigned short sel_call_gate = set_gdt_selector(GDT_CALL_GATE32,RPL_PRIVILEGE_USER);
volatile unsigned short sel_ring3_code = set_gdt_selector(GDT_RING3_CODE,RPL_PRIVILEGE_USER);
volatile unsigned short sel_ring3_data = set_gdt_selector(GDT_RING3_DATA,RPL_PRIVILEGE_USER);
volatile unsigned short sel_tss_des = set_gdt_selector(GDT_TSS32_DESCRIPTOR,RPL_PRIVILEGE_USER);

volatile unsigned short sel_ldt_kernel_code = set_ldt_selector(LDT_KERNEL_CODE,RPL_PRIVILEGE_KRNL);
volatile unsigned short sel_ldt_kernel_data = set_ldt_selector(LDT_KERNEL_DATA,RPL_PRIVILEGE_KRNL);

void set_gdt(){
	__asm__ __volatile__("lgdt %0" : :"m"(gdtr));
}

void init_interrput(){
	disp_str("Initial Interrupt\n");
	init_8259A();
	set_idt();
}

void init_ldt(){
	disp_str("Init LDT\n");
	set_descriptor(&gdt_des[GDT_LDT_DESCRIPTOR],DPL_PRIVILEGE_KRNL,(u32)ldt_des,0x20,DA_LDT);
	__asm__ __volatile__("lldt %0"::"m"(sel_ldt_des));
}

void init_call_gate32(){
	disp_str("Init Call gate\n");
	set_gate32((volatile GATE32 *)&gdt_des[GDT_CALL_GATE32],DPL_PRIVILEGE_USER,(u32)ring0_exit,sel_kernel_code,DA_386CGate);

}

void init_tss32(){
	disp_str("Init Tss\n");
	set_tss32(&tss32);
	set_descriptor(&gdt_des[GDT_TSS32_DESCRIPTOR],DPL_PRIVILEGE_USER,(u32)&tss32, sizeof(tss32),DA_386TSS);
	__asm__ __volatile__("ltr %0"::"m"(sel_tss_des));
}

void set_descriptor(volatile DESCRIPTOR32 *pdescripot, u8 dpl, u32 base, u32 limit, u8 type){
	pdescripot->descriptor_bitmap.seg_limit_0 = limit&0xFFFF;
	pdescripot->descriptor_bitmap.seg_limit_1 = (limit>>16)&0xFFFF;
	pdescripot->descriptor_bitmap.base_0 = base&0xFFFF;
	pdescripot->descriptor_bitmap.base_1 = (base>>16)&0xFF;
	pdescripot->descriptor_bitmap.base_2 = (base>>24)&0xFF;
	pdescripot->descriptor_bitmap.dpl = dpl;
	pdescripot->descriptor_bitmap.type = type&0x1f;
}

void set_gate32(volatile GATE32 *pgate, u8 dpl, u32 offset, u16 selector, u8 type){
	pgate->gate32_bitmap.seg_s = selector;
	pgate->gate32_bitmap.offset_0 = offset&0xFFFF;
	pgate->gate32_bitmap.offset_1 = (offset>>16)&0xFF;
	pgate->gate32_bitmap.dpl = dpl;
	pgate->gate32_bitmap.type = type&0x1f;
	pgate->gate32_bitmap.p = 1;
}

void set_tss32(TSS32* ptss32){
	ptss32->ss0 = (u32)sel_kernel_data;
	ptss32->esp0 = (u32)&Ring0StackTop;
	ptss32->iobase = (u16)((u32)ptss32->iomap-(u32)ptss32);
	memset(ptss32->iomap,0,127);
	ptss32->iomap[127] = 0xff;

}

void protect_main(){
	disp_str("Enter Protect Main\n");
	init_interrput();
	init_ldt();
	init_call_gate32();
	init_tss32();
//	__asm__ __volatile__("int $0x80");
}

void ldt_function(){
	disp_str("Enter LDT function\n");
}

void ring0exit(){
	disp_str("Enter Ring0 again\n");
}
void ring3start(){
	volatile INCALL_INFO32 icall;
	disp_str("Enter Ring3\n");
	icall.offset = 0;
	icall.seg = sel_call_gate;
	__asm__ __volatile__("lcall *%0"::"m"(icall));
}

void ring3main(){
	volatile RING3_INFO32 stack_info;
	stack_info.ss = sel_ring3_data;
	stack_info.esp = (u32)&Ring3StackTop;
	stack_info.cs = sel_ring3_code;
	stack_info.eip = (u32)ring3start;
	__asm__ __volatile__("movw %0, %%ax"::"m"(stack_info.ss));
	__asm__ __volatile__("movw %ax, %ds");
	__asm__ __volatile__("pushl %0"::"m"(stack_info.ss)); //stack swith need every entry 4-bytes align
	__asm__ __volatile__("pushl %0"::"m"(stack_info.esp));
	__asm__ __volatile__("pushl %0"::"m"(stack_info.cs));
	__asm__ __volatile__("pushl %0"::"m"(stack_info.eip));
	__asm__ __volatile__(".byte 0xCB");
}

