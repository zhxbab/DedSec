BOOTLOADER_PATH = bootloader
MBR_BIN = ${BOOTLOADER_PATH}/MBR.bin 
BOOT_BIN = ${BOOTLOADER_PATH}/boot.bin
SETUP_BIN = ${BOOTLOADER_PATH}/setup.bin
KERNEL_PATH = kernel
KERNEL_BIN = ${KERNEL_PATH}/kernel.bin
KERNEL_NUM = 50 #must same as KERNEL_NUM in boot.inc
KERNEL_SINDEX = 67
IMG_PATH = ../img
IMG = DedSec.img
TOOLS_PATH = ../debug
BS_PATH = ../bs

.PHONY: all
all: 
	$(MAKE) all -C  ${BOOTLOADER_PATH}
	$(MAKE) all -C  ${KERNEL_PATH}
	dd if=${MBR_BIN} of=${IMG_PATH}/${IMG} bs=446 count=1 conv=notrunc
	dd if=${BOOT_BIN} of=${IMG_PATH}/${IMG} bs=512 count=1 seek=63 conv=notrunc
	dd if=${SETUP_BIN} of=${IMG_PATH}/${IMG} bs=512 count=3 seek=64 conv=notrunc
	dd if=${KERNEL_BIN} of=${IMG_PATH}/${IMG} bs=512 count=${KERNEL_NUM} seek=${KERNEL_SINDEX} conv=notrunc
	
.PHONY: clean
clean:
	$(MAKE) clean -C  ${BOOTLOADER_PATH}
	$(MAKE) clean -C  ${KERNEL_PATH}
	rm -f ${IMG_PATH}/${IMG} bochsout.txt
	cp ${IMG_PATH}/DedSec_ori.img ${IMG_PATH}/${IMG}
	
.PHONY: run
run:
	${TOOLS_PATH}/bochs -qf ${BS_PATH}/DedSec.bochsrc
