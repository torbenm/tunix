QEMU=qemu-system-riscv32

KERNEL_FILE=kernel/out/kernel.elf

# Apps to 'install'
SHELL_FILE=apps/shell/out/shell.bin.o
BG_FILE=apps/bg/out/bg.bin.o

LLVM_HOME=/opt/homebrew/opt/llvm/bin


DEPS=$(KERNEL_FILE)
APPS=$(SHELL_FILE)

disk.tar: $(BG_FILE)
	rm -f disk/bg
	# cp $(BG_FILE) disk/bg
	cd disk && tar cf ../disk.tar --format=ustar *

.phony: $(KERNEL_FILE)
$(KERNEL_FILE): $(APPS)
	$(MAKE) -C kernel out/kernel.elf APPS=$(APPS)

.phony: $(SHELL_FILE)
$(SHELL_FILE):
	$(MAKE) -C apps/shell out/shell.bin.o

.phony: $(BG_FILE)
$(BG_FILE):
	$(MAKE) -C apps/bg out/bg.bin.o

.phony: boot
boot: $(DEPS) disk.tar
	$(QEMU) -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -d unimp,guest_errors,int,cpu_reset -D qemu.log \
    -drive id=drive0,file=disk.tar,format=raw,if=none \
    -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
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
	make -C apps/bg clean
	make -C shrdlib clean
	rm -f disk.tar