TOOLS_PATH = ../../debug
ASM_COMPLIER = ${TOOLS_PATH}/nasm
BOCHS = ${TOOLS_PATH}/bochs

MBR.bin:
	${ASM_COMPLIER} MBR.asm -o MBR.bin
boot.bin:
	${ASM_COMPLIER} boot.asm -o boot.bin
setup.bin:
	${ASM_COMPLIER} setup.asm -o setup.bin	
	
.PHONY: all
all:MBR.bin boot.bin setup.bin
	
.PHONY: clean
clean:
	rm -f *.bin
