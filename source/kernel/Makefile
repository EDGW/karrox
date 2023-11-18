include ../build.mak
.PHONY:clean all

SOURCE_FILES := $(call rwildcard,,*.c) $(call rwildcard,,*.cpp) $(call rwildcard,,*.S)\
				$(call rwildcard,,*.c.gen.py) $(call rwildcard,,*.cpp.gen.py) $(call rwildcard,,*.S.gen.py)
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