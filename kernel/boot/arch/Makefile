.PHONY: all
export INCLUDES += -I $(shell pwd)
objdir:
	mkdir -p $(OBJ_DIR)
all:objdir
	@echo make $(ARCH) boot
	make -C $(ARCH) arch
	rm -rf $(OBJ_DIR)/*
	cp -r $(ARCH)/$(OBJ_DIR)/* $(OBJ_DIR)/


clean:
	make -C $(ARCH) clean
	rm -rf $(OBJ_DIR)