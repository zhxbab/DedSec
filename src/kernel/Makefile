TOOLS_PATH = ../../debug
ASM_COMPLIER = ${TOOLS_PATH}/nasm
INC_PATH = inc
CC = gcc
LD = ld
CFLAGS = -c -m32
KERNEL_SADDR = 0x9000
KERNEL_SINDEX = 67
KERNEL_NUM = 20
LDFLAGS = -s -m elf_i386 -Ttext ${KERNEL_SADDR}
LIBS_PATH = ../lib/
OBJS = kernel.o 
LIBS_OBJS = ${LIBS_PATH}/test.o
BIN = kernel.bin
COPYFLAGS =  -R .comment -R .note -R .eh_frame -O binary

kernel.o: 
	${ASM_COMPLIER} -f elf kernel.asm -o $@
protect.o:
	${CC} ${CFLAGS} protect.c -o $@

${LIBS_OBJS}:
	$(MAKE) all -C  ${LIBS_PATH}
		
$(BIN):${LIBS_OBJS} $(OBJS) protect.o
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJS) ${LIBS_OBJS} protect.o
	objcopy ${COPYFLAGS} kernel.elf $(BIN)
	objdump -xD kernel.elf > kernel.map

.PHONY: all
all:${BIN}	
	
.PHONY: clean
clean:
	rm -f *.bin *.o *.map *.elf
	$(MAKE) clean -C  ${LIBS_PATH}
