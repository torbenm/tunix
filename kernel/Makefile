objects := $(patsubst %.c,%.o,$(wildcard *.c))

OUT_DIR=out
INC_DIR=includes

LLVM_HOME=/opt/homebrew/opt/llvm/bin
CC=$(LLVM_HOME)/clang
LINKER_FLAGS=-Wl,-Tkernel.ld -Wl,-Map=$(OUT_DIR)/kernel.map
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib -I$(INC_DIR)

_DEPS = $(wildcard $(INC_DIR)/*.h)
DEPS = $(patsubst %,%,$(_DEPS))

_OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
OBJ = $(patsubst %,$(OUT_DIR)/%,$(_OBJ))

_APPS = $(patsubst apps/%,../apps/%,$(APPS))

$(OUT_DIR)/%.o: %.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_DIR)/kernel.elf: $(OBJ)
	$(CC) $(CFLAGS) $(_APPS) $^ -o $(OUT_DIR)/kernel.elf $(LINKER_FLAGS)


.PHONY: clean
clean:
	rm -f *.o
	rm -f **/*.o
	rm -rfd arch/*/out