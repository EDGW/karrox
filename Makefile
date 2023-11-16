export OBJ_DIR		= .obj
export CC			= gcc
export CC_ARGS_32	= -m32 -Werror
export CC_ARGS_COMMON	= $(CC_ARGS_32)
export ARCH			= x86
export KERNEL_FILE  = $(OBJ_DIR)/kernel.sys
ISOROOT				= run/isofile
.PHONY: all,init
all:
	make -C kernel all
	mkdir -p run
	rm -rf ./run/*
	mkdir -p $(ISOROOT)/boot/grub
	cp kernel/$(KERNEL_FILE)  $(ISOROOT)/boot/
	cp grub.cfg $(ISOROOT)/boot/grub/
	grub-mkrescue -o ./run/karrox.iso $(ISOROOT)

run:all
	bochs -q -f ./bochs.bxrc
init:
	mkdir -p dev
	dd if=/dev/urandom of=./dev/ata1master.img bs=512 count=30720
	dd if=/dev/urandom of=./dev/ata1slave.img bs=512 count=30720
	dd if=/dev/urandom of=./dev/ata2master.img bs=512 count=30720
	dd if=/dev/urandom of=./dev/ata2slave.img bs=512 count=30720
	dd if=/dev/urandom of=./dev/usbdisk.img bs=512 count=30720
mount:
	losetup /dev/loop0 run/karrox.iso
	mkdir -p mountediso
	mount /dev/loop0 ./mountediso
umount:
	umount ./mountediso
	losetup -d /dev/loop0
	rm -rf ./mountediso
publish: all
	mkdir -p publish
	cp ./run/karrox.iso publish/karrox-$(ARCH).iso
clean:
	make -C kernel clean
	rm -rf $(OBJ_DIR)
	rm -rf run
	@echo Everything is cleaned