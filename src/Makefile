TOOLS_PATH = ../debug
ASM_COMPLIER = ${TOOLS_PATH}/nasm
BOCHS = ${TOOLS_PATH}/bochs
IMG_PATH = ../img
IMG = DedSec.img
INC_PATH = inc
BS_PATH = ../bs

MBR.bin:${INC_PATH}/MBR.inc
	${ASM_COMPLIER} MBR.asm -o MBR.bin
boot.bin:
	${ASM_COMPLIER} boot.asm -o boot.bin
	
.PHONY: all
all:MBR.bin boot.bin
	dd if=MBR.bin of=${IMG_PATH}/${IMG} bs=446 count=1 conv=notrunc
	dd if=boot.bin of=${IMG_PATH}/${IMG} bs=512 count=1 seek=63 conv=notrunc

.PHONY: clean
clean:
	rm -f *.bin ${IMG_PATH}/${IMG}
	cp ${IMG_PATH}/DedSec_ori.img ${IMG_PATH}/${IMG}
	
.PHONY: run
run:
	${TOOLS_PATH}/bochs -qf ${BS_PATH}/DedSec.bochsrc
