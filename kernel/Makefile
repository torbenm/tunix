objects := $(patsubst %.c,%.o,$(wildcard *.c))

OUT_DIR=out
INC_DIR=includes

LIB_DIR=../shrdlib
LIB_INC_DIR=$(LIB_DIR)/includes
LIB_OUT_DIR=$(LIB_DIR)/out

LLVM_HOME=/opt/homebrew/opt/llvm/bin
CC=$(LLVM_HOME)/clang
LINKER_FLAGS=-Wl,-Tkernel.ld -Wl,-Map=$(OUT_DIR)/kernel.map
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib -I$(INC_DIR) -I$(LIB_INC_DIR)

DEPS = $(wildcard $(INC_DIR)/*.h) $(wildcard $(LIB_INC_DIR)/*.h)

OBJ = $(patsubst %.c,$(OUT_DIR)/%.o,$(wildcard *.c))

LIB_OBJS = $(patsubst $(LIB_DIR)/%.c,$(LIB_OUT_DIR)/%.o,$(wildcard $(LIB_DIR)/*.c))

_APPS = $(patsubst apps/%,../apps/%,$(APPS))


$(LIB_OUT_DIR)/%.o: 
	make -C $(LIB_DIR) $(patsubst $(LIB_DIR)/%,%,$@)

$(OUT_DIR)/%.o: %.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_DIR)/kernel.elf: $(OBJ) $(LIB_OBJS)
	$(CC) $(CFLAGS) $(_APPS) $^ -o $(OUT_DIR)/kernel.elf $(LINKER_FLAGS)


.PHONY: clean
clean:
	rm -f *.o
	rm -f **/*.o
	rm -rfd arch/*/out