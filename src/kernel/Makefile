TOOLS_PATH = ../../debug
ASM_COMPLIER = ${TOOLS_PATH}/nasm
INC_PATH = inc
CC = gcc
LD = ld
CFLAGS = -c -static -nostdlib -fno-builtin -m32 -Os
KERNEL_SADDR = 0x9000
LDFLAGS = -s -m elf_i386 -Ttext ${KERNEL_SADDR}
DEBUG_LDFLAGS = -m elf_i386 -Ttext ${KERNEL_SADDR}
LIBS_PATH = ../lib/
OBJS = kernel.o protect.o int.o real_int.o
LIBS_OBJS = ${LIBS_PATH}/stdlib.o ${LIBS_PATH}/klib.o ${LIBS_PATH}/mem.o ${LIBS_PATH}/string.o
BIN = kernel.bin
COPYFLAGS =  -R .comment -R .note -R .eh_frame -O binary

kernel.o: 
	${ASM_COMPLIER} -f elf kernel.asm -o $@
int.o: 
	${ASM_COMPLIER} -f elf int.asm -o $@
protect.o:
	${CC} ${CFLAGS} protect.c -o $@
real_int.o:
	${CC} ${CFLAGS} real_int.c -o $@

${LIBS_OBJS}:
	$(MAKE) all -C  ${LIBS_PATH}
		
$(BIN):${LIBS_OBJS} $(OBJS) 
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJS) ${LIBS_OBJS}
	$(LD) $(DEBUG_LDFLAGS) -o kernel_d.elf $(OBJS) ${LIBS_OBJS}
	objcopy ${COPYFLAGS} kernel.elf $(BIN)
	objdump -xsD kernel_d.elf > kernel_d.map

.PHONY: all
all:${BIN}	
	
.PHONY: clean
clean:
	rm -f *.bin *.o *.map *.elf
	$(MAKE) clean -C  ${LIBS_PATH}
