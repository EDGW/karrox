include build.mak

PARTS = arch kernel
OUTPUT_DIR := .bin
TEMP_KERNEL_FILE := $(OBJ_DIR)/kernel.sys
KERNEL_FILE := $(OUTPUT_DIR)/kernel.sys

.PHONY:force
force:

all: $(patsubst %,$(OBJ_DIR)/%,$(PARTS)) force 
	make link

link:force
	@mkdir -p $(OUTPUT_DIR)
	ld $(LINKER_ARGS) -T config/link.ld -o $(TEMP_KERNEL_FILE) $(call rwildcard,$(OBJ_DIR),**.o)
	grub-file --is-x86-multiboot2 $(TEMP_KERNEL_FILE)
	cp $(TEMP_KERNEL_FILE) $(KERNEL_FILE)

clean: $(patsubst %,clean-$(OBJ_DIR)/%,$(PARTS)) force

$(OBJ_DIR)/%: force
	@mkdir -p $@
	@make -C $(patsubst $(OBJ_DIR)/%,%,$@) build
	@cp -r $(patsubst $(OBJ_DIR)/%,%,$@)/$(OBJ_DIR)/* $@

clean-$(OBJ_DIR)/%: force
	rm -rf $(OBJ_DIR)
	rm -rf $(OUTPUT_DIR)
	@make -C $(patsubst clean-$(OBJ_DIR)/%,%,$@) clean
	@echo Everything was cleaned.