.PHONY: all
all:objdir
	make -C boot all
	make -C drive all
	cp -r boot/$(OBJ_DIR)/* $(OBJ_DIR)/boot
	cp -r drive/$(OBJ_DIR)/* $(OBJ_DIR)/drive
	make $(KERNEL_FILE)
SUBDIRS = $(shell find $(OBJ_DIR) -maxdepth 10 -type d)
FILES = $(foreach dir,$(SUBDIRS),$(wildcard $(dir)/*.o))
export INCLUDES += -I $(shell pwd)
$(KERNEL_FILE):
	ld -m elf_i386 -T ./__linker.ld -o $(KERNEL_FILE) -nostdlib $(FILES)
	grub-file --is-x86-multiboot $(KERNEL_FILE)	# verifying
objdir:
	mkdir -p $(OBJ_DIR)
	rm -rf $(OBJ_DIR)/*
	mkdir -p $(OBJ_DIR)/boot
	mkdir -p $(OBJ_DIR)/drive
clean:
	make -C boot clean
	make -C drive clean
	rm -rf $(OBJ_DIR)