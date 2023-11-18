include ../build.mak
.PHONY:clean all
ARCH = x86

SOURCE_FILES := $(call rwildcard,$(ARCH),*.c) $(call rwildcard,$(ARCH),*.cpp) $(call rwildcard,$(ARCH),*.S)\
				$(call rwildcard,$(ARCH),*.c.gen.py) $(call rwildcard,$(ARCH),*.cpp.gen.py) $(call rwildcard,$(ARCH),*.S.gen.py)
OBJECT_FILES = 	$(filter %.o,$(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCE_FILES)))	\
				$(filter %.o,$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SOURCE_FILES)))	\
				$(filter %.o,$(patsubst %.S,$(OBJ_DIR)/%.o,$(SOURCE_FILES)))	\
				$(filter %.o,$(patsubst %.c.gen.py,$(OBJ_DIR)/%.o,$(SOURCE_FILES))) \
				$(filter %.o,$(patsubst %.cpp.gen.py,$(OBJ_DIR)/%.o,$(SOURCE_FILES))) \
				$(filter %.o,$(patsubst %.S.gen.py,$(OBJ_DIR)/%.o,$(SOURCE_FILES))) 


build:$(OBJECT_FILES)

clean:
	rm -rf $(GEN_DIR)
	rm -rf $(OBJ_DIR)