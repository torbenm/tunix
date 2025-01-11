objects := $(patsubst %.c,%.o,$(wildcard *.c))

OUT_DIR=out
INC_DIR=includes

LLVM_HOME=/opt/homebrew/opt/llvm/bin
OBJCOPY=$(LLVM_HOME)/llvm-objcopy

LINKER_FLAGS=-Wl,-T../lib/app.ld -Wl,-Map=$(OUT_DIR)/shell.map
CC=$(LLVM_HOME)/clang
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32 -ffreestanding -nostdlib -I$(INC_DIR) -I../lib/includes

_DEPS = $(wildcard $(INC_DIR)/*.h)
DEPS = $(patsubst %,%,$(_DEPS)) ../lib/includes/std.h

_OBJ = $(patsubst %.c,%.o,$(wildcard *.c))
OBJ = $(patsubst %,$(OUT_DIR)/%,$(_OBJ)) ../lib/out/app.o

../lib/out/%.o: 
	make -C ../lib $(patsubst ../lib/%,%,$@)

$(OUT_DIR)/%.o: %.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUT_DIR)/shell.bin.o: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(OUT_DIR)/shell.elf $(LINKER_FLAGS)
	$(OBJCOPY) --set-section-flags .bss=alloc,contents -O binary $(OUT_DIR)/shell.elf $(OUT_DIR)/shell.bin
	$(OBJCOPY) -Ibinary -Oelf32-littleriscv $(OUT_DIR)/shell.bin $(OUT_DIR)/shell.bin.o


.PHONY: clean
clean:
	rm -rf out