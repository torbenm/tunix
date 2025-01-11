
# Other options: arm
ARCH?=riscv32
QEMU=qemu-system-$(ARCH)
objects := $(patsubst %.c,%.o,$(wildcard *.c))

ARCH_HOME=arch/$(ARCH)

OUT_DIR=$(ARCH_HOME)/out
INC_DIR=src
SRC_DIR=src

LLVM_HOME=/opt/homebrew/opt/llvm/bin
CC=$(LLVM_HOME)/clang
LOADER_FLAGS=-Wl,-T$(ARCH_HOME)/kernel.ld -Wl,-Map=$(OUT_DIR)/kernel.map
CFLAGS=-std=c11 -O2 -g3 -Wall -Wextra --target=$(ARCH) -ffreestanding -nostdlib -I$(INC_DIR)

_DEPS = $(wildcard $(INC_DIR)/*.h)
DEPS = $(patsubst %,%,$(_DEPS))

_OBJ = $(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/*.c))
OBJ = $(patsubst $(SRC_DIR)/%,$(OUT_DIR)/%,$(_OBJ))

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	mkdir -p "$(OUT_DIR)"
	$(CC) -c -o $@ $< $(CFLAGS)

%/out/$(ARCH)/%.o:
	echo $^
	echo $@


.phony: build
build: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(OUT_DIR)/kernel.elf $(LOADER_FLAGS)

run: build
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel $(OUT_DIR)/kernel.elf

objdump:
	$(LLVM_HOME)/llvm-objdump -d $(OUT_DIR)/kernel.elf

nm:
	$(LLVM_HOME)/llvm-nm $(OUT_DIR)/kernel.elf

addr2line:
	$(LLVM_HOME)/llvm-addr2line -e $(OUT_DIR)/kernel.elf $(line)


.PHONY: clean
clean:
	rm -f *.o
	rm -f **/*.o
	rm -rfd arch/*/out