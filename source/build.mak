# This makefile model defined some typical build-target templates

# default object file folder path
OBJ_DIR = .obj

# default generated-code path
GEN_DIR = .gen

# default gcc args
CC_ARGS := -Werror -m32 -static -nostdlib -fno-builtin
LINKER_ARGS := -m elf_i386 --oformat elf32-i386

INCLUDE_DIR := ../lib
INCLUDE_ARG := -I $(INCLUDE_DIR)

# recursive wildcard
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Assembly File
$(OBJ_DIR)/%.o: %.S $(call rwildcard,$(INCLUDE_DIR),**.h) $(call rwildcard,,**.h)
	@mkdir -p $(dir $@)
	gcc $(CC_ARGS) -c -DASM_FILE $(INCLUDE_ARG) -o $@ $<

# C File
$(OBJ_DIR)/%.o: %.c $(call rwildcard,$(INCLUDE_DIR),**.h) $(call rwildcard,,**.h)
	@mkdir -p $(dir $@)
	gcc $(CC_ARGS) -c -DC_FILE $(INCLUDE_ARG) -o $@ $<

# C++ File
$(OBJ_DIR)/%.o: %.cpp $(call rwildcard,$(INCLUDE_DIR),**.h) $(call rwildcard,,**.h)
	@mkdir -p $(dir $@)
	gcc $(CC_ARGS) -c -DCPP_FILE $(INCLUDE_ARG) -o $@ $<

# Asm Python Gen
$(OBJ_DIR)/%.o: %.S.gen.py $(call rwildcard,$(INCLUDE_DIR),**.h) $(call rwildcard,,**.h)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(patsubst .obj/%.o,.gen/%.S,$@))
	python3 $< >$(patsubst .obj/%.o,.gen/%.S,$@)
	gcc $(CC_ARGS) -c -DASM_FILE $(INCLUDE_ARG) -o $@ $(patsubst .obj/%.o,.gen/%.S,$@)

# C Python Gen
$(OBJ_DIR)/%.o: %.c.gen.py $(call rwildcard,$(INCLUDE_DIR),**.h) $(call rwildcard,,**.h)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(patsubst .obj/%.o,.gen/%.c,$@))
	python3 $< >$(patsubst .obj/%.o,.gen/%.c,$@)
	gcc $(CC_ARGS) -c -DC_FILE $(INCLUDE_ARG) -o $@ $(patsubst .obj/%.o,.gen/%.c,$@)

# C++ Python Gen
$(OBJ_DIR)/%.o: %.cpp.gen.py $(call rwildcard,$(INCLUDE_DIR),**.h) $(call rwildcard,,**.h)
	@mkdir -p $(dir $@)
	@mkdir -p $(dir $(patsubst .obj/%.o,.gen/%.cpp,$@))
	python3 $< >$(patsubst .obj/%.o,.gen/%.cpp,$@)
	gcc $(CC_ARGS) -c -DCPP_FILE $(INCLUDE_ARG) -o $@ $(patsubst .obj/%.o,.gen/%.cpp,$@)