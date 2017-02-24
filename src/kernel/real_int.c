#include "inc/int.h"
#include "../lib/inc/klib.h"
#include "inc/protect.h"

extern unsigned short sel_kernel_code;
extern unsigned short sel_8253_tss_des;
extern void	divide_error();
extern void	single_step_exception();
extern void	nmi();
extern void	breakpoint_exception();
extern void	overflow();
extern void	bounds_check();
extern void	inval_opcode();
extern void	copr_not_available();
extern void	double_fault();
extern void	copr_seg_overrun();
extern void	inval_tss();
extern void	segment_not_present();
extern void	stack_exception();
extern void	general_protection();
extern void	page_fault();
extern void	copr_error();
extern void int_8259_0();
extern void int_8259_1();
extern void int_8259_2();
extern void int_8259_3();
extern void int_8259_4();
extern void int_8259_5();
extern void int_8259_6();
extern void int_8259_7();
extern void int_8259_8();
extern void int_8259_9();
extern void int_8259_10();;
extern void int_8259_11();
extern void int_8259_12();
extern void int_8259_13();
extern void int_8259_14();
extern void int_8259_15();
extern void int_80h();

GATE32 idt[256];

IDTR32 idtr = {
		.limit = IDT_SIZE,
		.base = (unsigned int)idt
};

/*======================================================================*
                            init_8259A
 *======================================================================*/
PUBLIC void init_8259A()
{
	disp_str("Init 8259A\n");
	/* Master 8259, ICW1. */
	outb(INT_M_CTL,	0x11);

	/* Slave  8259, ICW1. */
	outb(INT_S_CTL,	0x11);

	/* Master 8259, ICW2. 设置 '主8259' 的中断入口地址为 0x20. */
	outb(INT_M_CTLMASK,	INT_VECTOR_IRQ0);

	/* Slave  8259, ICW2. 设置 '从8259' 的中断入口地址为 0x28 */
	outb(INT_S_CTLMASK,	INT_VECTOR_IRQ8);

	/* Master 8259, ICW3. IR2 对应 '从8259'. */
	outb(INT_M_CTLMASK,	0x4);

	/* Slave  8259, ICW3. 对应 '主8259' 的 IR2. */
	outb(INT_S_CTLMASK,	0x2);

	/* Master 8259, ICW4. */
	outb(INT_M_CTLMASK,	0x1);

	/* Slave  8259, ICW4. */
	outb(INT_S_CTLMASK,	0x1);

	/* Master 8259, OCW1.  */
	outb(INT_M_CTLMASK,	0xFD);//enable keyboard
	//outb(INT_M_CTLMASK,	0xFC);//enable keyboard and 8253

	/* Slave  8259, OCW1.  */
	outb(INT_S_CTLMASK,	0xFF);
}

/*======================================================================*
                           spurious_irq
 *======================================================================*/
PUBLIC void spurious_irq(int irq)
{
        disp_str("spurious_irq: ");
        disp_int_hex(irq);
        disp_str("\n");
}

PUBLIC void set_idt(){
	disp_str("Start Set Idt\n");
	set_idt_single(0x80,(u32)int_80h,0,sel_kernel_code);
	set_idt_single(0x0,(u32)divide_error,0,sel_kernel_code);
	set_idt_single(0x1+INT_VECTOR_IRQ0,(u32)int_8259_1,0,sel_kernel_code);
	set_idt_single_task(0x0+INT_VECTOR_IRQ0,0,sel_8253_tss_des);
	__asm__ __volatile__("lidt %0" : :"m"(idtr));
	__asm__ __volatile__("sti");
	disp_str("Enable interrput\n");
}
PUBLIC void set_idt_single(u8 index, u32 handler, u8 dpl, u16 sel){
	idt[index].gate32_bitmap.seg_s = sel;
	idt[index].gate32_bitmap.dpl = dpl;
	idt[index].gate32_bitmap.offset_0 = (u16)handler&0xFFFF;
	idt[index].gate32_bitmap.offset_1 = (u16)((handler>>16)&0xFFFF);
	idt[index].gate32_bitmap.p = 1;
	idt[index].gate32_bitmap.type = DA_386IGate;
}

PUBLIC void set_idt_single_task(u8 index, u8 dpl, u16 sel){
	idt[index].gate32_bitmap.seg_s = sel;
	idt[index].gate32_bitmap.dpl = dpl;
	idt[index].gate32_bitmap.p = 1;
	idt[index].gate32_bitmap.type = DA_TaskGate;
}

void real_divide_error(){

}
void real_single_step_exception(){

}
void real_nmi(){

}
void real_breakpoint_exception(){

}
void real_overflow(){

}
void real_bounds_check(){

}
void real_inval_opcode(){

}
void real_copr_not_available(){

}
void real_double_fault(){

}
void real_copr_seg_overrun(){

}
void real_inval_tss(){

}
void real_segment_not_present(){

}
void real_stack_exception(){

}
void real_general_protection(){

}
void real_page_fault(){

}
void real_copr_error(){

}
void real_int_8259_0(){
	disp_str("..");
	sent_eoi(0);
}
void real_int_8259_1(){
	volatile u8 data = get_keyboard_data();
	sent_eoi(1);
	disp_int_hex((int)data);
	disp_char('\n');
	disp_char(data);

}
void real_int_8259_2(){

}
void real_int_8259_3(){

}
void real_int_8259_4(){

}
void real_int_8259_5(){

}
void real_int_8259_6(){

}
void real_int_8259_7(){

}
void real_int_8259_8(){

}
void real_int_8259_9(){

}
void real_int_8259_10(){

}
void real_int_8259_11(){

}
void real_int_8259_12(){

}
void real_int_8259_13(){

}
void real_int_8259_14(){

}
void real_int_8259_15(){

}
void real_int_80h(){
	disp_str("Enter 80h handler\n");
}

void sent_eoi(u8 index){
	if(index < 8){
		outb(INT_M_CTL, EOI_8259);
	}
	else{
		outb(INT_S_CTL, EOI_8259);
	}
}

u8 get_keyboard_data(){
	volatile u8 status = inb(KEYBOARD_CMD_IO);
	volatile u8 data;
	//if(status == 0x1){
	data = inb(KEYBOARD_DATA_IO);
	//}
	return data;
}

void enable_8253(){
	outb(INT_M_CTLMASK,	0xFC);//enable keyboard and 8253
}

void set_8253(volatile u32 latch){
	__asm__ __volatile__("cli");
	__asm__ __volatile__("movb $0x36, %al");
	__asm__ __volatile__("movl $0x43, %edx");
	__asm__ __volatile__("outb %al, %dx");
	__asm__ __volatile__("movl %0, %%eax"::"m"(latch));
	__asm__ __volatile__("movl $0x40, %edx");
	__asm__ __volatile__("outb %al, %dx");
	__asm__ __volatile__("movb %ah, %al");
	__asm__ __volatile__("outb %al, %dx");
}
