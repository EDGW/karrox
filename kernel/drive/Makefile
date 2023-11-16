.PHONY: all objdir		# here is phony targets
__FILES	 =  			# object codes
FILES	 = $(patsubst %, ${OBJ_DIR}/%.o, $(__FILES))

objdir:
	mkdir -p $(OBJ_DIR)
	rm -rf $(OBJ_DIR)/*
	mkdir -p $(OBJ_DIR)/pci
	
all: objdir $(FILES)
#	(optional) make in child directory
	make -C pci all
	cp -r pci/$(OBJ_DIR)/* $(OBJ_DIR)/pci

$(OBJ_DIR)/%.o : %.S objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<
$(OBJ_DIR)/%.o : %.c objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<
$(OBJ_DIR)/%.o : %.cpp objdir
	$(CC) $(INCLUDES) $(CC_ARGS_COMMON) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)
	make -C pci clean