ASM         = nasm
CC          = gcc
LD          = ld
GRUB        = grub-mkrescue
QEMU        = qemu-system-i386

# 32-bit setting
CFLAGS      = -m32 -ffreestanding -O2 -fno-pic -fno-stack-protector
LDFLAGS     = -m elf_i386 -Ttext 0x1000 -e _start --oformat binary -nostdlib

# Paths
BOOT_SRC    = bootloader/boot.asm
BOOT_BIN    = build/boot.bin
MULTIBOOT_SRC = kernel/multiboot_header.S
MULTIBOOT_OBJ = build/multiboot_header.o
KERNEL_SRC  = kernel/kernel.c
KERNEL_OBJ  = build/kernel.o
KERNEL_BIN  = build/kernel.bin

ISO_DIR     = iso
ISO         = build/myos.iso

# Targets
all: $(ISO)

# Compile the bootloader
$(BOOT_BIN): $(BOOT_SRC)
	mkdir -p build
	$(ASM) -f bin $< -o $@

# Compile the multiboot header
$(MULTIBOOT_OBJ): $(MULTIBOOT_SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Compile the kernel
$(KERNEL_OBJ): $(KERNEL_SRC)
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel with multiboot header
$(KERNEL_BIN): $(MULTIBOOT_OBJ) $(KERNEL_OBJ)
	mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $^

# Create the ISO image
$(ISO): $(BOOT_BIN) $(KERNEL_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(BOOT_BIN) $(ISO_DIR)/boot/boot.bin
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	@echo set timeout=0                         > $(ISO_DIR)/boot/grub/grub.cfg
	@echo set default=0                         >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo menuentry "HDI OS" {                  >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo   multiboot /boot/kernel.bin          >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo   boot                                >> $(ISO_DIR)/boot/grub/grub.cfg
	@echo }                                     >> $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB) -o $@ $(ISO_DIR)

# Run the OS in QEMU
qemu: all
	$(QEMU) -cdrom $(ISO)

# Clean up build artifacts
clean:
	rm -rf build
	rm -rf $(ISO_DIR)/boot
	rm -f $(ISO)
