run: all
	cd ../out && bochs
all: os-image

booting.bin:
	cd ../boot_sector && $(MAKE)

kernel.bin: kernel_entry.o kernel.o
	i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary

kernel.o : main_kernel.cpp
	i686-elf-g++ -ffreestanding -c main_kernel.cpp -o kernel.o

kernel_entry.o : kernel_entry.asm
	nasm kernel_entry.asm -f elf -o kernel_entry.o

os-image: kernel.o kernel_entry.o booting.bin kernel.bin
	cd ../out && cat ../src/booting.bin ../src/kernel.bin > os-image