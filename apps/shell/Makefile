objects := $(patsubst %.c,%.o,$(wildcard *.c))

OUT_DIR=out
INC_DIR=includes

LIB_DIR=../../shrdlib
LIB_INC_DIR=$(LIB_DIR)/includes
LIB_OUT_DIR=$(LIB_DIR)/out

APP_LIB_DIR=../lib
APP_LIB_INC_DIR=$(APP_LIB_DIR)/includes
APP_LIB_OUT_DIR=$(APP_LIB_DIR)/out

LLVM_HOME=/opt/homebrew/opt/llvm/bin
OBJCOPY=$(LLVM_HOME)/llvm-objcopy

LINKER_FLAGS=-Wl,-T../lib/app.ld -Wl,-Map=$(OUT_DIR)/shell.map
CC=$(LLVM_HOME)/clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib -I$(INC_DIR) -I$(APP_LIB_INC_DIR) -I$(LIB_INC_DIR)

DEPS = $(wildcard $(INC_DIR)/*.h) $(wildcard $(LIB_INC_DIR)/*.h) $(wildcard $(APP_LIB_INC_DIR)/*.h)

LIB_OBJS = $(patsubst $(LIB_DIR)/%.c,$(LIB_OUT_DIR)/%.o,$(wildcard $(LIB_DIR)/*.c))
APP_LIB_OBJS = $(patsubst $(APP_LIB_DIR)/%.c,$(APP_LIB_OUT_DIR)/%.o,$(wildcard $(APP_LIB_DIR)/*.c))

OBJ = $(patsubst %.c,$(OUT_DIR)/%.o,$(wildcard *.c))

$(APP_LIB_OUT_DIR)/%.o: 
	make -C $(APP_LIB_DIR) $(patsubst $(APP_LIB_DIR)/%,%,$@)

$(LIB_OUT_DIR)/%.o: 
	make -C $(LIB_DIR) $(patsubst $(LIB_DIR)/%,%,$@)

$(OUT_DIR)/%.o: %.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_DIR)/shell.bin.o: $(OBJ) $(LIB_OBJS) $(APP_LIB_OBJS)
	echo $(OBJ) $(LIB_OBJS) $(APP_LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $(OUT_DIR)/shell.elf $(LINKER_FLAGS)
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary $(OUT_DIR)/shell.elf $(OUT_DIR)/shell.bin
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv $(OUT_DIR)/shell.bin $(OUT_DIR)/shell.bin.o


.PHONY: clean
clean:
	rm -rf out