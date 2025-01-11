QEMU=qemu-system-riscv32

KERNEL_FILE=kernel/out/kernel.elf

# Apps to 'install'
SHELL_FILE=apps/shell/out/shell.bin.o

LLVM_HOME=/opt/homebrew/opt/llvm/bin


DEPS=$(KERNEL_FILE)
APPS=$(SHELL_FILE)

.phony: $(KERNEL_FILE)
$(KERNEL_FILE): $(APPS)
	$(MAKE) -C kernel out/kernel.elf APPS=$(APPS)

.phony: $(SHELL_FILE)
$(SHELL_FILE):
	$(MAKE) -C apps/shell out/shell.bin.o

.phony: boot
boot: $(DEPS)
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel $(KERNEL_FILE)

.phony: run
fresh: 
	$(MAKE) clean
	$(MAKE) boot

objdump:
	$(LLVM_HOME)/llvm-objdump -d $(file)

nm:
	$(LLVM_HOME)/llvm-nm $(file)

addr2line:
	$(LLVM_HOME)/llvm-addr2line -e $(OUT_DIR)/kernel.elf $(line)

clean:
	make -C kernel clean
	make -C apps/lib clean
	make -C apps/shell clean