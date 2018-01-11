KERNEL_DIR := kernel
BOOT_DIR := boot
CC := i686-elf-g++
LD := i686-elf-ld
CPPFLAGS := -ffreestanding -g

all: os-image


os-image: kernel.bin booting.bin
	cat $(KERNEL_DIR)/booting.bin $(KERNEL_DIR)/kernel.bin > boot/os-image
	cd $(BOOT_DIR) && bochs

booting.bin:
	cd boot_sector && $(MAKE)

kernel.bin:
	$(CC) $(CPPFLAGS) -c $(KERNEL_DIR)/kernel_entry_start.s -o $(KERNEL_DIR)/kernel_entry_start.o
	$(CC) $(CPPFLAGS) -g -c $(KERNEL_DIR)/kernel.cpp -o $(KERNEL_DIR)/kernel.o

	$(LD) -Ttext 0x1000 $(KERNEL_DIR)/kernel_entry_start.o $(KERNEL_DIR)/kernel.o --oformat binary -o $(KERNEL_DIR)/kernel.bin
