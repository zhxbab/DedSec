#include "inc/klib.h"
#include "inc/stdlib.h"

volatile unsigned int disp_pos = 0;
/*======================================================================*
                               disp_int
 *======================================================================*/
PUBLIC void disp_int_hex(int input)
{
	char output[16];
	itoa(output, input);
	disp_str(output);
}

/* ========================================================================
;                  void disp_str(char * info);
; ========================================================================
*/
PUBLIC void	disp_str(u8 * pszInfo){
	char * p = pszInfo;
	while(*p != '\0'){
		disp_char(*p);
		p++;
	}
}

PUBLIC void	disp_char(u8 data){
	unsigned int temp_disp_pos = disp_pos;
	u8 color = 0x0f;
	u16 cinfo = 0x0;
	if(data != '\n')
	{
		cinfo = (color<<8)+data;
		__asm__ __volatile__("movl %0, %%edi" : :"m"(disp_pos));
		__asm__ __volatile__("movw %0, %%ax" : :"m"(cinfo));
		__asm__ __volatile__("movw %ax, %gs:0(%edi)"); //no constraint asm need one %, but constraint asm need two %%
		disp_pos = disp_pos + 2;
		temp_disp_pos = disp_pos;
	}
	else{
		//every line has 80 point, every point need to 2bytes.
		temp_disp_pos =((u32)(temp_disp_pos/160) + 1)*160;
		disp_pos = temp_disp_pos;
	}
	sync_cursor(disp_pos);
}


PUBLIC void outb(u16 io_num, u8 io_data){
	__asm__ __volatile__("movw %0, %%dx" : :"m"(io_num));
	__asm__ __volatile__("movb %0, %%al" : :"m"(io_data));
	__asm__ __volatile__("out %al, %dx");
	__asm__ __volatile__("nop;nop;nop;nop");
}


PUBLIC u8 inb(u16 io_num){
	volatile u8 data;
	__asm__ __volatile__("movw %0, %%dx" : :"m"(io_num));
	__asm__ __volatile__("xor %eax,%eax; in %dx, %al");
	__asm__ __volatile__("movb %%al, %0" : :"m"(data));
	__asm__ __volatile__("nop;nop;nop;nop");
	return data;
}

PUBLIC void sync_cursor(u32 disp_pos){
	volatile u16 temp_disp_pos = (u16)(disp_pos/2);
	outb(CURSOR_INDEX_IO,CURSOR_DATA_H);
	outb(CURSOR_DATA_IO,(u8)((temp_disp_pos>>8)&0xFF));
	outb(CURSOR_INDEX_IO,CURSOR_DATA_L);
	outb(CURSOR_DATA_IO,(u8)(temp_disp_pos&0xFF));
}


