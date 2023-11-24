include build.mak

PARTS = arch kernel lib
OUTPUT_DIR 			:= .bin
TEMP_KERNEL_FILE 	:= $(OBJ_DIR)/kernel.sys
KERNEL_FILE 		:= $(OUTPUT_DIR)/kernel.sys
ISO_DIR				:= .iso
ISO_KERNEL_PATH		:= $(ISO_DIR)/boot/karrox_kernel.sys
DEVICE_DIR			:= .dev

.PHONY:force
force:

all: $(patsubst %,$(OBJ_DIR)/%,$(PARTS)) force 
	make link
	make img

run:all
	qemu-system-x86_64 -cpu core2duo -m 128 \
	-drive format=raw,media=cdrom,file=.bin/karrox.iso \

bochs:all
	bochs -f ./config/bochs.bxrc -q

img:force
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_FILE) $(ISO_DIR)/boot/
	cp config/grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(OUTPUT_DIR)/karrox.iso $(ISO_DIR)
	@if [ ! -e $(DIR_TEST) ];then dd if=/dev/urandom of=$(DEVICE_DIR)/sata.img count=65536;fi
	@if [ ! -e $(DIR_TEST) ];then dd if=/dev/urandom of=$(DEVICE_DIR)/nvm.img count=65536;fi
	@if [ ! -e $(DIR_TEST) ];then dd if=/dev/urandom of=$(DEVICE_DIR)/ide.img bs=512 count=65536;fi

SUBDIRS = $(shell find $(OBJ_DIR) -maxdepth 10 -type d)
FILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.o))
link:force
	@mkdir -p $(OUTPUT_DIR)
	ld -m elf_i386 -T ./__link.ld -o $(KERNEL_FILE) -nostdlib $(FILES)
	grub-file --is-x86-multiboot2 $(KERNEL_FILE)

clean: $(patsubst %,clean-$(OBJ_DIR)/%,$(PARTS)) force

$(OBJ_DIR)/%: force
	@mkdir -p $@
	@make -C $(patsubst $(OBJ_DIR)/%,%,$@) build
	@cp -r $(patsubst $(OBJ_DIR)/%,%,$@)/$(OBJ_DIR)/* $@

clean-$(OBJ_DIR)/%: force
	rm -rf $(OBJ_DIR)
	rm -rf $(OUTPUT_DIR)
	rm -rf $(ISO_DIR)
	rm -rf $(DEVICE_DIR)
	@make -C $(patsubst clean-$(OBJ_DIR)/%,%,$@) clean
	@echo Everything was cleaned.