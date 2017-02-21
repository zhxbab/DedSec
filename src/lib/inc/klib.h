#ifndef	_KLIB_H_
#define	_KLIB_H_
#include "type.h"
#include "const.h"

#define CURSOR_INDEX_IO 0x3d4
#define CURSOR_DATA_IO 0x3d5
#define KEYBOARD_CMD_IO 0x64
#define KEYBOARD_DATA_IO 0x60
#define CURSOR_DATA_H 0xe
#define CURSOR_DATA_L 0xf

PUBLIC void	disp_char(u8 data);
PUBLIC void disp_int_hex(int input);
PUBLIC void	disp_str(u8 * pszInfo);
PUBLIC u8   inb(u16 io_num);
PUBLIC void outb(u16 io_num, u8 io_data);
PUBLIC void sync_cursor(u32 disp_pos);

#endif /* _KLIB_H_ */
