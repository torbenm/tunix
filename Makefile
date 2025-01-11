QEMU=qemu-system-riscv32

KERNEL_FILE=kernel/out/kernel.elf
LLVM_HOME=/opt/homebrew/opt/llvm/bin


DEPS=$(KERNEL_FILE)

.phony:
$(KERNEL_FILE): 
	$(MAKE) -C kernel out/kernel.elf

.phony: boot
boot: $(DEPS)
	echo $(DEPS)
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -kernel $(KERNEL_FILE)

objdump:
	$(LLVM_HOME)/llvm-objdump -d $(OUT_DIR)/kernel.elf

nm:
	$(LLVM_HOME)/llvm-nm $(OUT_DIR)/kernel.elf

addr2line:
	$(LLVM_HOME)/llvm-addr2line -e $(OUT_DIR)/kernel.elf $(line)