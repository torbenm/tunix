objects := $(patsubst %.c,%.o,$(wildcard *.c))

OUT_DIR=out
INC_DIR=includes

LLVM_HOME=/opt/homebrew/opt/llvm/bin
CC=$(LLVM_HOME)/clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib -I$(INC_DIR)

_DEPS = $(wildcard $(INC_DIR)/*.h)
DEPS = $(patsubst %,%,$(_DEPS))

_OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
OBJ = $(patsubst %,$(OUT_DIR)/%,$(_OBJ))

$(OUT_DIR)/%.o: %.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)


.PHONY: clean
clean:
	rm -rf out