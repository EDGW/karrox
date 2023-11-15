# make sure that 'header' is the first object
__FILES	 = header boot
FILES	 = $(patsubst %, ${OBJ_DIR}/%.o, $(__FILES))
objdir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

arch: $(FILES)

$(OBJ_DIR)/%.o : %.S objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<
$(OBJ_DIR)/%.o : %.c objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<
$(OBJ_DIR)/%.o : %.cpp objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<