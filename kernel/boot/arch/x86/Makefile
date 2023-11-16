# make sure that 'header' is the first object
__FILES	 = header boot
FILES	 = $(patsubst %, ${OBJ_DIR}/%.o, $(__FILES))
objdir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf .gen
gen:
	mkdir -p .gen
	mkdir -p $(OBJ_DIR)
	python3 ./intr_handler_S_gen.py>.gen/intr_handler.S
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o .obj/intr_handler.o .gen/intr_handler.S

arch:gen $(FILES)
	

$(OBJ_DIR)/%.o : %.S objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<
$(OBJ_DIR)/%.o : %.c objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<
$(OBJ_DIR)/%.o : %.cpp objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<