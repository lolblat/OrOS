KERNEL_DIR := kernel
BOOT_DIR := boot
CC := i686-elf-g++
LD := i686-elf-ld
CPPFLAGS := -Wall -ffreestanding -g -fno-exceptions
CPP_SOURCES = $(wildcard drivers/*.cpp cpu/*.cpp Util/*.cpp)
CPP_HEADERS = $(wildcard drivers/*.h cpu/*.h Util/*.h)
ASM_FILES = $(wildcard cpu/*.asm )
OBJ = ${CPP_SOURCES:.cpp=.o}
ASM_OBJ_FILE = ${ASM_FILES:.asm=.o}

all: os-image

os-image: kernel.bin booting.bin
	cat $(KERNEL_DIR)/booting.bin $(KERNEL_DIR)/kernel.bin > boot/os-image
	cd $(BOOT_DIR) && bochs

booting.bin:
	cd boot_sector && $(MAKE)

kernel.bin: kernel/kernel_entry_start.s ${ASM_OBJ_FILE} $(OBJ) kernel/kernel.cpp
	$(CC) $(CPPFLAGS) -c $(KERNEL_DIR)/kernel_entry_start.s -o $(KERNEL_DIR)/kernel_entry_start.o
	$(CC) $(CPPFLAGS) -c $(KERNEL_DIR)/kernel.cpp -o $(KERNEL_DIR)/kernel.o
	$(LD) -Ttext 0x1000 $(KERNEL_DIR)/kernel_entry_start.o $(ASM_OBJ_FILE) $(KERNEL_DIR)/kernel.o $(OBJ) --oformat binary -o $(KERNEL_DIR)/kernel.bin

%.o: %.cpp $(CPP_HEADERS)
	$(CC) $(CPPFLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@
	# Cant use extern in bin files, and becuase of that change to elf file format.

remove_all:
	cd kernel && rm *.o *.bin
	cd drivers && rm *.o
	cd cpu && rm *.o
