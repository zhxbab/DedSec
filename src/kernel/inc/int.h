#ifndef	_INT_H_
#define	_INT_H_

#include "../../lib/inc/type.h"
#include "../../lib/inc/const.h"

/* 中断向量 */
#define	INT_VECTOR_DIVIDE		0x0
#define	INT_VECTOR_DEBUG		0x1
#define	INT_VECTOR_NMI			0x2
#define	INT_VECTOR_BREAKPOINT		0x3
#define	INT_VECTOR_OVERFLOW		0x4
#define	INT_VECTOR_BOUNDS		0x5
#define	INT_VECTOR_INVAL_OP		0x6
#define	INT_VECTOR_COPROC_NOT		0x7
#define	INT_VECTOR_DOUBLE_FAULT		0x8
#define	INT_VECTOR_COPROC_SEG		0x9
#define	INT_VECTOR_INVAL_TSS		0xA
#define	INT_VECTOR_SEG_NOT		0xB
#define	INT_VECTOR_STACK_FAULT		0xC
#define	INT_VECTOR_PROTECTION		0xD
#define	INT_VECTOR_PAGE_FAULT		0xE
#define	INT_VECTOR_COPROC_ERR		0x10

/* 中断向量 */
#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28

#define INT_M_CTL     0x20 /* I/O port for interrupt controller       <Master> */
#define INT_M_CTLMASK 0x21 /* setting bits in this port disables ints <Master> */
#define INT_S_CTL     0xA0 /* I/O port for second interrupt controller<Slave>  */
#define INT_S_CTLMASK 0xA1 /* setting bits in this port disables ints <Slave>  */

#define EOI_8259	0x20

/* 8253/8254 PIT (Programmable Interval Timer) */
#define TIMER0         0x40 /* I/O port for timer channel 0 */
#define TIMER_MODE     0x43 /* I/O port for timer mode control */
#define RATE_GENERATOR 0x34 /* 00-11-010-0 :
			     * Counter0 - LSB then MSB - rate generator - binary
			     */
#define TIMER_0_LATCH	11930/* clock frequency for timer0(8253) in PC and AT (100Hz)*/
#define TIMRE_0_FREQ	100  /* clock freq (software settable on IBM-PC) */


void real_divide_error();
void real_single_step_exception();
void real_nmi();
void real_breakpoint_exception();
void real_overflow();
void real_bounds_check();
void real_inval_opcode();
void real_copr_not_available();
void real_double_fault();
void real_copr_seg_overrun();
void real_inval_tss();
void real_segment_not_present();
void real_stack_exception();
void real_general_protection();
void real_page_fault();
void real_copr_error();
void real_int_8259_0();
void real_int_8259_1();
void real_int_8259_2();
void real_int_8259_3();
void real_int_8259_4();
void real_int_8259_5();
void real_int_8259_6();
void real_int_8259_7();
void real_int_8259_8();
void real_int_8259_9();
void real_int_8259_10();
void real_int_8259_11();
void real_int_8259_12();
void real_int_8259_13();
void real_int_8259_14();
void real_int_8259_15();
void real_int_80h();
void sent_eoi(u8 index);
u8 get_keyboard_data();
void enable_8253();
void set_8253(volatile u32 latch);
PUBLIC void init_8259A();
PUBLIC void set_idt_single(u8 index, u32 handler, u8 dpl, u16 sel);
PUBLIC void set_idt_single_task(u8 index, u8 dpl, u16 sel);
PUBLIC void set_idt();
PUBLIC void spurious_irq(int irq);

#endif /* _INT_H_ */
