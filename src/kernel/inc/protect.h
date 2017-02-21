#ifndef	_PROTECT_H_
#define	_PROTECT_H_
#include "../../lib/inc/type.h"
/* GDT 和 IDT 中描述符的个数 */
#define	GDT_SIZE	128*8
#define	IDT_SIZE	256*8

/* 权限 */
#define	DPL_PRIVILEGE_KRNL	0
#define	DPL_PRIVILEGE_TASK	1
#define	DPL_PRIVILEGE_USER	3

/* 权限 */
#define	RPL_PRIVILEGE_KRNL	0
#define	RPL_PRIVILEGE_TASK	1
#define	RPL_PRIVILEGE_USER	3

/* 存储段描述符类型值说明 */
#define	DA_DR			0x10	/* 存在的只读数据段类型值		*/
#define	DA_DRW			0x12	/* 存在的可读写数据段属性值		*/
#define	DA_DRWA			0x13	/* 存在的已访问可读写数据段类型值	*/
#define	DA_C			0x18	/* 存在的只执行代码段属性值		*/
#define	DA_CR			0x1A	/* 存在的可执行可读代码段属性值		*/
#define	DA_CCO			0x1C	/* 存在的只执行一致代码段属性值		*/
#define	DA_CCOR			0x1E	/* 存在的可执行可读一致代码段属性值	*/

/* 系统段描述符类型值说明 */
#define	DA_LDT			0x02	/* 局部描述符表段类型值			*/
#define	DA_TaskGate		0x05	/* 任务门类型值				*/
#define	DA_386TSS		0x09	/* 可用 386 任务状态段类型值		*/
#define	DA_386CGate		0x0C	/* 386 调用门类型值			*/
#define	DA_386IGate		0x0E	/* 386 中断门类型值			*/
#define	DA_386TGate		0x0F	/* 386 陷阱门类型值			*/

#define SEG_PRESENT		0x1
#define SEG_NO_PRESENT	0x0

#define SEG_OPERATION_SIZE_16	0x0
#define SEG_OPERATION_SIZE_32	0x1

#define SEG_GRANULARITY_1		0x0
#define SEG_GRANULARITY_4K		0x1

#define SEG_SIZE_TYPE_PROTECT		0x2
#define SEG_SIZE_TYPE_LONG			0x1
#define SEG_SIZE_TYPE_COM			0x0
#define SEG_SIZE_TYPE_INVALID		0x0

#define SEG_AVL_SET					0x1
#define SEG_AVL_NO_SET				0x0

#define GATE32_NO_COUNT				0x0

/* GDT MAP */
#define GDT_RESERVERD				0x0
#define GDT_KERNEL_CODE				0x1
#define GDT_KERNEL_DATA				0x2
#define GDT_VRAM					0x3
#define GDT_LDT_DESCRIPTOR  		0x4
#define GDT_CALL_GATE32				0x5 // for switch to kernel
#define GDT_RING3_CODE 				0x6
#define GDT_RING3_DATA 				0x7
#define GDT_TSS32_DESCRIPTOR 		0x8
/* LDT MAP */
#define LDT_KERNEL_CODE				0x1
#define LDT_KERNEL_DATA				0x2

#define init_descriptor32_data(index, data_0_v, data_1_v)\
	[index].descriptor32_data[0] = data_0_v,\
	[index].descriptor32_data[1] = data_1_v

#define init_gate32_data(index, data_0_v, data_1_v)\
	[index].gate32_data[0] = data_0_v,\
	[index].gate32_data[1] = data_1_v

#define init_descriptor32_bitmap(index, seg_limit_0_v, base_0_v, base_1_v, type_v, dpl_v, p_v,seg_limit_1_v, avl_v, size_type_v, g_v, base_2_v)\
	[index].bitmap.seg_limit_0 = seg_limit_0_v,\
	[index].bitmap.base_0 = base_0_v,\
	[index].bitmap.base_1 = base_1_v,\
	[index].bitmap.type = type_v,\
	[index].bitmap.dpl = dpl_v,\
	[index].bitmap.p = p_v,\
	[index].bitmap.seg_limit_1 = seg_limit_1_v,\
	[index].bitmap.avl = avl_v,\
	[index].bitmap.size_type = size_type_v,\
	[index].bitmap.g = g_v,\
	[index].bitmap.base_2 = base_2_v

#define init_gate32_bitmap(index, offset_0_v, seg_s_v, count_v, type_v, dpl_v, p_v, offset_1_v)\
	[index].bitmap.offset_0 = offset_0_v,\
	[index].bitmap.seg_s = seg_s_v,\
	[index].bitmap.count = count_v,\
	[index].bitmap.dummy_0 = 0x0,\
	[index].bitmap.type = type_v,\
	[index].bitmap.dpl = dpl_v,\
	[index].bitmap.p = p_v,\
	[index].bitmap.offset_1 = offset_1_v

#define get_selector(index,location,rpl)\
	((index<<3)&0xff + (location<<2)&0xf + (rpl&0xf))

/* 存储段描述符/系统段描述符 */
typedef union s_descriptor32		/* 共 8 个字节 */
{
	unsigned int descriptor32_data[2];
	struct descriptor_bitmap_s{
		unsigned short seg_limit_0	:16;
		unsigned short base_0	:16;
		unsigned short base_1	:8;
		unsigned short type		:5; //include s bit
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short seg_limit_1		:4;
		unsigned short avl		:1;
		unsigned short size_type		:2; //include l bit and d/b bit
		unsigned short g		:1;
		unsigned short base_2	:8;
	}descriptor_bitmap;
}DESCRIPTOR32;

typedef union s_gate32		/* 共 8 个字节 */
{
	unsigned int gate32_data[2];
	struct gate32_bitmap_s{
		unsigned short offset_0	:16;
		unsigned short seg_s	:16;
		unsigned short count	:5;
		unsigned short dummy_0	:3;
		unsigned short type		:5;
		unsigned short dpl		:2;
		unsigned short p		:1;
		unsigned short offset_1	:16;
	}gate32_bitmap;
}GATE32;

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

typedef struct s_ring3_info32{
	u16 ss;
	u32 esp;
	u16 cs;
	u32 eip;
}RING3_INFO32;

typedef struct s_incall_info32{
	u32 offset;
	u16 seg;
}INCALL_INFO32;

void set_ldt_descriptor(DESCRIPTOR32 *pdescripot, u8 dpl, u32 base, u32 limit);
void set_call_gate32(DESCRIPTOR32 *pdescripot, u8 dpl, u32 offset, u16 selector);
void init_tss32();
void set_tss32(TSS32* ptss32);

#endif /* _PROTECT_H_ */
