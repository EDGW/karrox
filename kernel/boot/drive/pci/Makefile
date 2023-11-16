.PHONY: all objdir
__FILES	 = pci
FILES	 = $(patsubst %, ${OBJ_DIR}/%.o, $(__FILES))

objdir:
	mkdir -p $(OBJ_DIR)
	rm -rf $(OBJ_DIR)/*
	mkdir -p $(OBJ_DIR)
	
all: objdir $(FILES)

$(OBJ_DIR)/%.o : %.S objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<
$(OBJ_DIR)/%.o : %.c objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<
$(OBJ_DIR)/%.o : %.cpp objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)