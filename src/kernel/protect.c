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



extern u32 Ring0StackTop;

DESCRIPTOR32 gdt_des[16] = {
		init_descriptor32_data(GDT_RESERVERD,0x0,0x0),
		init_descriptor32_bitmap(GDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel Code Seg
		init_descriptor32_bitmap(GDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // Kernel Data Seg
		init_descriptor32_bitmap(GDT_VRAM,0xffff,0x8000,0x0B,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_1,0x00), // VRAM Seg
		init_descriptor32_bitmap(GDT_TASKA_TSS32_DESCRIPTOR,0x0,0x0,0x0,DA_386TSS,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_INVALID,SEG_GRANULARITY_1,0x00), // available TaskA_Tss32 Descriptor
		init_descriptor32_bitmap(GDT_TASKB_TSS32_DESCRIPTOR,0x0,0x0,0x0,DA_386TSS,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_INVALID,SEG_GRANULARITY_1,0x00), // available TaskB_Tss32 Descriptor
		init_descriptor32_bitmap(GDT_TIMER0_TSS32_DESCRIPTOR,0x0,0x0,0x0,DA_386TSS,DPL_PRIVILEGE_KRNL,\
				SEG_PRESENT,0x0,SEG_AVL_SET,SEG_SIZE_TYPE_INVALID,SEG_GRANULARITY_1,0x00), // available 8253 Tss Descriptor
		init_descriptor32_bitmap(GDT_LDT_TASKA_DESCRIPTOR,0xffff,0x0000,0x00,DA_LDT,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // TaskA LDT
		init_descriptor32_bitmap(GDT_LDT_TASKB_DESCRIPTOR,0xffff,0x0000,0x00,DA_LDT,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // TaskB LDT
};

GDTR32 gdtr = {
		.limit = GDT_SIZE,
		.base = (unsigned int)gdt_des
};


DESCRIPTOR32 ldt_taska_des[LDT_SIZE] = {
		init_descriptor32_bitmap(LDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT TaskA Code Seg
		init_descriptor32_bitmap(LDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT TaskA Data Seg
};
DESCRIPTOR32 ldt_taskb_des[LDT_SIZE] = {
		init_descriptor32_bitmap(LDT_KERNEL_CODE,0xffff,0x0000,0x00,DA_CR,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT TaskA Code Seg
		init_descriptor32_bitmap(LDT_KERNEL_DATA,0xffff,0x0000,0x00,DA_DRW,DPL_PRIVILEGE_USER,\
				SEG_PRESENT,0xf,SEG_AVL_SET,SEG_SIZE_TYPE_PROTECT,SEG_GRANULARITY_4K,0x00), // LDT TaskA Data Seg
};
TSS32 tss32, TaskA_tss32, Timer0_tss32, TaskB_tss32;
TR tr;
u32 taskb_stack[100], taska_stack[100];
u32 current_thread_id;
THREAD_FRAME Task_frame[3];
u32 tick = 0;

unsigned short sel_kernel_code = set_gdt_selector(GDT_KERNEL_CODE,RPL_PRIVILEGE_KRNL);
unsigned short sel_kernel_data = set_gdt_selector(GDT_KERNEL_DATA,RPL_PRIVILEGE_KRNL);
unsigned short sel_vram = set_gdt_selector(GDT_VRAM,RPL_PRIVILEGE_USER);

unsigned short sel_ldt_taska_code = set_ldt_selector(LDT_KERNEL_CODE,RPL_PRIVILEGE_USER);
unsigned short sel_ldt_taska_data = set_ldt_selector(LDT_KERNEL_DATA,RPL_PRIVILEGE_USER);
unsigned short sel_ldt_taskb_code = set_ldt_selector(LDT_KERNEL_CODE,RPL_PRIVILEGE_USER);
unsigned short sel_ldt_taskb_data = set_ldt_selector(LDT_KERNEL_DATA,RPL_PRIVILEGE_USER);
unsigned short sel_ldt_taska = set_gdt_selector(GDT_LDT_TASKA_DESCRIPTOR,RPL_PRIVILEGE_USER);
unsigned short sel_ldt_taskb = set_gdt_selector(GDT_LDT_TASKA_DESCRIPTOR,RPL_PRIVILEGE_USER);

unsigned short sel_taska_tss_des = set_gdt_selector(GDT_TASKA_TSS32_DESCRIPTOR,RPL_PRIVILEGE_USER);
unsigned short sel_taskb_tss_des = set_gdt_selector(GDT_TASKB_TSS32_DESCRIPTOR,RPL_PRIVILEGE_USER);
unsigned short sel_timer0_tss_des = set_gdt_selector(GDT_TIMER0_TSS32_DESCRIPTOR,RPL_PRIVILEGE_KRNL);

void set_gdt(){
	__asm__ __volatile__("lgdt %0" : :"m"(gdtr));
}

void init_interrput(){
	disp_str("Initial Interrupt\n");
	init_8259A();
	set_idt();
}

void init_ldt(u8 index, DESCRIPTOR32 * pdes, u16 sel, u8 dpl){
	disp_str("Init LDT\n");
	set_descriptor(&gdt_des[index],dpl,(u32)pdes,sizeof(pdes)*LDT_SIZE,DA_LDT);
	__asm__ __volatile__("lldt %0"::"m"(sel));
}


void init_tss32(TSS32 *ptss32, u16 ptss32_sel, u32 ptss32_gdt_index, u8 dpl, u8 flag){
	disp_str("Init Tss\n");
	set_tss32(ptss32);
	set_descriptor(&gdt_des[ptss32_gdt_index],dpl,(u32)ptss32, sizeof(TSS32),DA_386TSS);
	if(flag){
		__asm__ __volatile__("ltr %0"::"m"(ptss32_sel));
	}
}

void init_thread_chain(){
	Task_frame[1].ptss32 = &TaskA_tss32;
	Task_frame[1].pstack = taska_stack;
	Task_frame[2].ptss32 = &TaskB_tss32;
	Task_frame[2].pstack = taskb_stack;
	Task_frame[1].tick = 100;
	Task_frame[2].tick = 1;
	Task_frame[1].tss_sel = sel_taska_tss_des;
	Task_frame[2].tss_sel = sel_taskb_tss_des;
}

void set_task_tss(THREAD_FRAME* pthread_frame, u32 stack_size, u16 code_sel, u16 data_sel, u16 vram_sel, u32 eflags, void * pfunc, u16 sel_ldt){
	pthread_frame->ptss32->cs = code_sel;
	pthread_frame->ptss32->ds = data_sel;
	pthread_frame->ptss32->ss = data_sel;
	pthread_frame->ptss32->es = data_sel;
	pthread_frame->ptss32->gs = vram_sel;
	pthread_frame->ptss32->fs = data_sel;
	pthread_frame->ptss32->eip = (u32)pfunc;
	pthread_frame->ptss32->esp = (u32)(pthread_frame->pstack) + stack_size;
	pthread_frame->ptss32->flags = eflags;
	pthread_frame->ptss32->ldt = sel_ldt;
}

void set_descriptor(DESCRIPTOR32 *pdescripot, u8 dpl, u32 base, u32 limit, u8 type){
	pdescripot->descriptor_bitmap.seg_limit_0 = limit&0xFFFF;
	pdescripot->descriptor_bitmap.seg_limit_1 = (limit>>16)&0xFFFF;
	pdescripot->descriptor_bitmap.base_0 = base&0xFFFF;
	pdescripot->descriptor_bitmap.base_1 = (base>>16)&0xFF;
	pdescripot->descriptor_bitmap.base_2 = (base>>24)&0xFF;
	pdescripot->descriptor_bitmap.dpl = dpl;
	pdescripot->descriptor_bitmap.type = type&0x1f;
}

void set_gate32(GATE32 *pgate, u8 dpl, u32 offset, u16 selector, u8 type){
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
	volatile u32 latch = TIMER_0_LATCH;
	init_interrput();
	init_tss32(&Timer0_tss32,sel_timer0_tss_des,GDT_TIMER0_TSS32_DESCRIPTOR,DPL_PRIVILEGE_KRNL,0);
	set_timer0(latch);
	enable_timer0();
	ring3main();
	while(1);

}

void ring3main(){
	u32 excepted_id = 1;
	init_tss32(&TaskA_tss32,sel_taska_tss_des,GDT_TASKA_TSS32_DESCRIPTOR,DPL_PRIVILEGE_USER,1);
	init_tss32(&TaskB_tss32,sel_taskb_tss_des,GDT_TASKB_TSS32_DESCRIPTOR,DPL_PRIVILEGE_USER,1);
	init_ldt(GDT_LDT_TASKA_DESCRIPTOR,ldt_taska_des,sel_ldt_taska,DPL_PRIVILEGE_USER);
	init_ldt(GDT_LDT_TASKB_DESCRIPTOR,ldt_taskb_des,sel_ldt_taskb,DPL_PRIVILEGE_USER);
	init_thread_chain();
	set_task_tss(&Task_frame[2], sizeof(taskb_stack), sel_ldt_taskb_code, sel_ldt_taskb_data, sel_vram, 0x202, &TaskB, sel_ldt_taskb);
	set_task_tss(&Task_frame[1], sizeof(taska_stack), sel_ldt_taska_code, sel_ldt_taska_data, sel_vram, 0x202, &TaskA, sel_ldt_taska);
	start_task(&Task_frame[excepted_id],excepted_id);

}

void TaskA(){
	while(1){
		delay(50);
		disp_str("A");
	}
}

void TaskB(){
	while(1){
		delay(50);
		disp_str("B");
	}
}

void start_task(THREAD_FRAME* pframe, u32 id){
	pframe->thread_id = id;
	current_thread_id = id;
	pframe->stack_frame.ss = pframe->ptss32->ss;
	pframe->stack_frame.esp = pframe->ptss32->esp;
	pframe->stack_frame.cs = pframe->ptss32->cs;
	pframe->stack_frame.eip = pframe->ptss32->eip;
	Timer0_tss32.backlink = (u32)pframe->tss_sel;
	__asm__ __volatile__("lldt %0"::"m"(pframe->ptss32->ldt));
	__asm__ __volatile__("movw %0, %%ax"::"m"(pframe->stack_frame.ss));
	__asm__ __volatile__("movw %ax, %ds");
	__asm__ __volatile__("pushl %0"::"m"(pframe->stack_frame.ss)); //stack swtich need every entry 4-bytes align
	__asm__ __volatile__("pushl %0"::"m"(pframe->stack_frame.esp));
	__asm__ __volatile__("pushl %0"::"m"(pframe->stack_frame.cs));
	__asm__ __volatile__("pushl %0"::"m"(pframe->stack_frame.eip));
	__asm__ __volatile__("sti");
	__asm__ __volatile__(".byte 0xCB");
}

void restart_next_task(){
	int last_tick = 0;
	last_tick = tick++;
	if(last_tick > tick) tick = 0;
	real_int_8259_0();
	save_tss(&Task_frame[current_thread_id]);
	scheduler();

}

void scheduler(){
	if(Task_frame[current_thread_id].tick == 0){
		if(current_thread_id==1){
			Timer0_tss32.backlink = (u32)sel_taskb_tss_des;
			Task_frame[current_thread_id].tick = 100;
			current_thread_id  = 2;
		}
		else{
			if(current_thread_id==2){
				Timer0_tss32.backlink = (u32)sel_taska_tss_des;
				Task_frame[current_thread_id].tick = 1;
				current_thread_id  = 1;
			}
		}
	}
	else{
		Task_frame[current_thread_id].tick --;
	}
	__asm__ __volatile__("ltr %0"::"m"(sel_timer0_tss_des));
	//	__asm__ __volatile__("sti");
	__asm__ __volatile__("iret");
}

void save_tss(THREAD_FRAME *pTask_frame){

	pTask_frame->ptss32->ds = pTask_frame->stack_frame.ds;
	pTask_frame->ptss32->gs = pTask_frame->stack_frame.gs;
	pTask_frame->ptss32->es = pTask_frame->stack_frame.es;
	pTask_frame->ptss32->ss = pTask_frame->stack_frame.ss;
	pTask_frame->ptss32->fs = pTask_frame->stack_frame.fs;
	pTask_frame->ptss32->eax = pTask_frame->stack_frame.eax;
	pTask_frame->ptss32->ebp = pTask_frame->stack_frame.ebp;
	pTask_frame->ptss32->ebx = pTask_frame->stack_frame.ebx;
	pTask_frame->ptss32->ecx = pTask_frame->stack_frame.ecx;
	pTask_frame->ptss32->edx = pTask_frame->stack_frame.edx;
	pTask_frame->ptss32->esi = pTask_frame->stack_frame.esi;
	pTask_frame->ptss32->edi = pTask_frame->stack_frame.edi;
	pTask_frame->ptss32->esp = pTask_frame->stack_frame.esp;
	pTask_frame->ptss32->eip = pTask_frame->stack_frame.eip;
	pTask_frame->ptss32->cs = pTask_frame->stack_frame.cs;
	pTask_frame->ptss32->flags = pTask_frame->stack_frame.eflags;

}

u32 get_tick(){
	// use eax as input and output
	__asm__ __volatile__("mov $0x1, %eax");
	__asm__ __volatile__("int $0x80");
	__asm__ __volatile__("mov %%eax, %0"::"m"(tick));
	return tick;
}

void delay(u32 milli_sec){
	u32 tick_temp = 0;
	u32 target_tick = 0;
	tick_temp = get_tick();
	target_tick = tick_temp + (u32)(milli_sec/10);
	while(1){
		if(get_tick() > target_tick) break;
	}
}
