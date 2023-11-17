__FILES	 = multiboot header boot intr_handler io intr_c_handler
FILES	 = $(patsubst %, ${OBJ_DIR}/%.o, $(__FILES))
objdir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -rf .gen

arch: $(FILES)

.obj/intr_handler.o: ./intr_handler_S_gen.py
	mkdir -p .gen
	mkdir -p $(OBJ_DIR)
	python3 ./intr_handler_S_gen.py>.gen/intr_handler.S
	$(CC) $(INCLUDES) -I $(shell pwd) $(CC_ARGS_32) -c -o .obj/intr_handler.o .gen/intr_handler.S

$(OBJ_DIR)/%.o : %.S objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<
$(OBJ_DIR)/%.o : %.c objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<
$(OBJ_DIR)/%.o : %.cpp objdir
	$(CC) $(INCLUDES) $(CC_ARGS_32) -c -o $@ $<