#ifndef	_PROTECT_H_
#define	_PROTECT_H_
#include "../../lib/inc/type.h"
/* GDT 和 IDT 中描述符的个数 */
#define	GDT_SIZE	128*8
#define	IDT_SIZE	256*8

/* 权限 */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3
/* RPL */
#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_USER	SA_RPL3

#define des_bitmap(index, seg_limit_0_v, base_0_v, base_1_v, type_v, s_v, dpl_v, p_v,seg_limit_1_v, avl_v, l_v, db_v, g_v, base_2_v)\
	[index].bitmap.seg_limit_0 = seg_limit_0_v,\
	[index].bitmap.base_0 = base_0_v,\
	[index].bitmap.base_1 = base_1_v,\
	[index].bitmap.type = type_v,\
	[index].bitmap.s = s_v,\
	[index].bitmap.dpl = dpl_v,\
	[index].bitmap.p = p_v,\
	[index].bitmap.seg_limit_1 = seg_limit_1_v,\
	[index].bitmap.avl = avl_v,\
	[index].bitmap.l = l_v,\
	[index].bitmap.db = db_v,\
	[index].bitmap.g = g_v,\
	[index].bitmap.base_2 = base_2_v

#define int_gate32_bitmap(index, offset_0_v, seg_s_v, dpl_v, offset_1_v)\
	[index].bitmap.offset_0 = offset_0_v,\
	[index].bitmap.seg_s = seg_s_v,\
	[index].bitmap.dummy_0 = 0x0,\
	[index].bitmap.dummy_1 = 0x0,\
	[index].bitmap.type = 0x6,\
	[index].bitmap.d = 0x1,\
	[index].bitmap.dummy_2 = 0x0,\
	[index].bitmap.dpl = dpl_v,\
	[index].bitmap.p = 0x1,\
	[index].bitmap.offset_1 = offset_1_v

#define call_gate32_bitmap_des(index, offset_0_v, seg_s_v, dpl_v, offset_1_v)\
	[index].call_gate32_bitmap_des.offset_0 = offset_0_v,\
	[index].call_gate32_bitmap_des.seg_s = seg_s_v,\
	[index].call_gate32_bitmap_des.dummy_0 = 0x0,\
	[index].call_gate32_bitmap_des.dummy_1 = 0x0,\
	[index].call_gate32_bitmap_des.type = 0xC,\
	[index].call_gate32_bitmap_des.dummy_2 = 0x0,\
	[index].call_gate32_bitmap_des.dpl = dpl_v,\
	[index].call_gate32_bitmap_des.p = 0x1,\
	[index].call_gate32_bitmap_des.offset_1 = offset_1_v


#define tss32_bitmap_des(index, seg_limit_0_v, base_0_v, base_1_v, type_v, dpl_v, seg_limit_1_v, avl_v, g_v, base_2_v)\
	[index].tss32_bitmap_des.seg_limit_0 = seg_limit_0_v,\
	[index].tss32_bitmap_des.base_0 = base_0_v,\
	[index].tss32_bitmap_des.base_1 = base_1_v,\
	[index].tss32_bitmap_des.type = type_v,\
	[index].tss32_bitmap_des.dummy_1 = 0x0,\
	[index].tss32_bitmap_des.dpl = dpl_v,\
	[index].tss32_bitmap_des.p = 0x1,\
	[index].tss32_bitmap_des.seg_limit_1 = seg_limit_1_v,\
	[index].tss32_bitmap_des.avl = avl_v,\
	[index].tss32_bitmap_des.dummy_2 = 0x0,\
	[index].tss32_bitmap_des.g = g_v,\
	[index].tss32_bitmap_des.base_2 = base_2_v

/* 存储段描述符/系统段描述符 */
typedef union s_descriptor32		/* 共 8 个字节 */
{
	unsigned int data[2];
	struct bitmap_s{
		unsigned short seg_limit_0	:16;
		unsigned short base_0	:16;
		unsigned short base_1	:8;
		unsigned short type		:4;
		unsigned short s		:1;
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short seg_limit_1		:4;
		unsigned short avl		:1;
		unsigned short l		:1;
		unsigned short db		:1;
		unsigned short g		:1;
		unsigned short base_2	:8;
	}bitmap;
	struct call_gate32_bitmap_des_s{
		unsigned short offset_0	:16;
		unsigned short seg_s	:16;
		unsigned short dummy_0	:5;
		unsigned short dummy_1	:3;
		unsigned short type		:4;
		unsigned short dummy_2	:1;
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short offset_1	:16;
	}call_gate32_bitmap_des;
	struct tss32_bitmap_des_s{
		unsigned short seg_limit_0	:16;
		unsigned short base_0	:16;
		unsigned short base_1	:8;
		unsigned short type		:4;
		unsigned short dummy_1	:1;
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short seg_limit_1	:4;
		unsigned short avl	:1;
		unsigned short dummy_2	:2;
		unsigned short g	:1;
		unsigned short base_2	:8;
	}tss32_bitmap_des;
}DESCRIPTOR32;

typedef struct s_gdtr32{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed)) GDTR32;

typedef struct s_idtr32{
	unsigned short limit;
	unsigned int base;
}__attribute__((packed)) IDTR32;

typedef struct s_ldtr{
	unsigned short selector;
}LDTR;

typedef struct s_tr{
	unsigned short selector;
}TR;

typedef union s_int_gate32		/* 共 8 个字节 */
{
	unsigned int data[2];
	struct int_gate32_bitmap_s{
		unsigned short offset_0	:16;
		unsigned short seg_s	:16;
		unsigned short dummy_0	:5;
		unsigned short dummy_1	:3;
		unsigned short type		:3;
		unsigned short d		:1;
		unsigned short dummy_2	:1;
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short offset_1	:16;
	}bitmap;
}INT_GATE32;


typedef union s_call_gate32		/* 共 8 个字节 */
{
	unsigned int data[2];
	struct call_gate32_bitmap_s{
		unsigned short offset_0	:16;
		unsigned short seg_s	:16;
		unsigned short count	:5; //in call gate it is count
		unsigned short dummy_0	:3;
		unsigned short type		:4;
		unsigned short dummy_1	:1;
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short offset_1	:16;
	}call_gate32_bitmap;
}CALL_GATE32;


typedef struct s_tss32 {
	u32	backlink;
	u32	esp0;		/* stack pointer to use during interrupt */
	u32	ss0;		/*   "   segment  "  "    "        "     */
	u32	esp1;
	u32	ss1;
	u32	esp2;
	u32	ss2;
	u32	cr3;
	u32	eip;
	u32	flags;
	u32	eax;
	u32	ecx;
	u32	edx;
	u32	ebx;
	u32	esp;
	u32	ebp;
	u32	esi;
	u32	edi;
	u32	es;
	u32	cs;
	u32	ss;
	u32	ds;
	u32	fs;
	u32	gs;
	u32	ldt;
	u16	trap;
	u16	iobase;	/* I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 */
	u8	iomap[128]; //io map
}TSS32;


void set_ldt_descriptor(DESCRIPTOR32 *pdescripot, u8 dpl, u32 base, u32 limit);
void set_call_gate32(DESCRIPTOR32 *pdescripot, u8 dpl, u32 offset, u16 selector);
void init_tss32();
void set_tss32(TSS32* ptss32);
typedef struct stack_info32{
	u16 ss;
	u32 esp;
	u16 cs;
	u32 eip;
}STACK_INFO32;


typedef struct icall32_s{
	u32 offset;
	u16 seg;
}ICALL32;
///* 门描述符 */
//typedef struct s_gate32
//{
//	u16	offset_low;	/* Offset Low */
//	u16	selector;	/* Selector */
//	u8	dcount;		/* 该字段只在调用门描述符中有效。如果在利用
//				   调用门调用子程序时引起特权级的转换和堆栈
//				   的改变，需要将外层堆栈中的参数复制到内层
//				   堆栈。该双字计数字段就是用于说明这种情况
//				   发生时，要复制的双字参数的数量。*/
//	u8	attr;		/* P(1) DPL(2) DT(1) TYPE(4) */
//	u16	offset_high;	/* Offset High */
//}GATE32;


//
//
///* GDT */
///* 描述符索引 */
//#define	INDEX_DUMMY		0	// ┓
//#define	INDEX_FLAT_C		1	// ┣ LOADER 里面已经确定了的.
//#define	INDEX_FLAT_RW		2	// ┃
//#define	INDEX_VIDEO		3	// ┛
///* 选择子 */
//#define	SELECTOR_DUMMY		   0		// ┓
//#define	SELECTOR_FLAT_C		0x08		// ┣ LOADER 里面已经确定了的.
//#define	SELECTOR_FLAT_RW	0x10		// ┃
//#define	SELECTOR_VIDEO		(0x18+3)	// ┛<-- RPL=3
//
//#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
//#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW
//
//
///* 描述符类型值说明 */
//#define	DA_32			0x4000	/* 32 位段				*/
//#define	DA_LIMIT_4K		0x8000	/* 段界限粒度为 4K 字节			*/
//#define	DA_DPL0			0x00	/* DPL = 0				*/
//#define	DA_DPL1			0x20	/* DPL = 1				*/
//#define	DA_DPL2			0x40	/* DPL = 2				*/
//#define	DA_DPL3			0x60	/* DPL = 3				*/
///* 存储段描述符类型值说明 */
//#define	DA_DR			0x90	/* 存在的只读数据段类型值		*/
//#define	DA_DRW			0x92	/* 存在的可读写数据段属性值		*/
//#define	DA_DRWA			0x93	/* 存在的已访问可读写数据段类型值	*/
//#define	DA_C			0x98	/* 存在的只执行代码段属性值		*/
//#define	DA_CR			0x9A	/* 存在的可执行可读代码段属性值		*/
//#define	DA_CCO			0x9C	/* 存在的只执行一致代码段属性值		*/
//#define	DA_CCOR			0x9E	/* 存在的可执行可读一致代码段属性值	*/
///* 系统段描述符类型值说明 */
//#define	DA_LDT			0x82	/* 局部描述符表段类型值			*/
//#define	DA_TaskGate		0x85	/* 任务门类型值				*/
//#define	DA_386TSS		0x89	/* 可用 386 任务状态段类型值		*/
//#define	DA_386CGate		0x8C	/* 386 调用门类型值			*/
//#define	DA_386IGate		0x8E	/* 386 中断门类型值			*/
//#define	DA_386TGate		0x8F	/* 386 陷阱门类型值			*/

#endif /* _PROTECT_H_ */
