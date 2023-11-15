.PHONY: all objdir
export INCLUDES += -I $(shell pwd)
__FILES	 = boot print
FILES	 = $(patsubst %, ${OBJ_DIR}/%.o, $(__FILES))
objdir:
	mkdir -p $(OBJ_DIR)
	rm -rf $(OBJ_DIR)/*
	mkdir -p $(OBJ_DIR)/arch

all: objdir $(FILES)
	make -C arch all
	cp -r arch/$(OBJ_DIR)/* $(OBJ_DIR)/arch
clean:
	make -C arch clean
	rm -rf $(OBJ_DIR)



$(OBJ_DIR)/%.o : %.S objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<
$(OBJ_DIR)/%.o : %.c objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<
$(OBJ_DIR)/%.o : %.cpp objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<