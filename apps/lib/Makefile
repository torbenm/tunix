objects := $(patsubst %.c,%.o,$(wildcard *.c))

OUT_DIR=out
INC_DIR=includes
LIB_DIR=../../shrdlib
LIB_INC_DIR=$(LIB_DIR)/includes

LLVM_HOME=/opt/homebrew/opt/llvm/bin
CC=$(LLVM_HOME)/clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib -I$(INC_DIR) -I$(LIB_INC_DIR)

DEPS = $(wildcard $(INC_DIR)/*.h) $(wildcard $(LIB_INC_DIR)/*.h)

OBJ = $(patsubst %.c,$(OUT_DIR)/%.c,$(wildcard *.c))

$(OUT_DIR)/%.o: %.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -rf out