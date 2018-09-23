//
// Created by lolblat on 10/01/18.
//
extern "C"
{


    #include "../drivers/screen.h"
    #include "../cpu/interrupts/isr.h"
    #include "../cpu/interrupts/idt.h"
    #include "../cpu/interrupts/timer.h"
    //#include "../drivers/keyboard.h"
    #include  "../Util/Util.h"
    #include "../cpu/MemoryDetector.h"
    #include "../MemoryAllocation/BitMapAllocator.h"
    #include "../MemoryAllocation/Paging.h"
    #include "../MemoryAllocation/MemoryManager.h"
#include "../drivers/ATA.h"
#include "../FileSystemEXT2/EXT2System.h"
#include "../FileSystemEXT2/VFS.h"
#include "../tasks/user_mode.hpp"
#include "../cpu/gdt.h"
    void kernel_main(BootingInfo&,u32,u32);
}

void kernel_main(BootingInfo& info, u32 physical_end, u32 virtual_end)
{
    Paging p = Paging();
    CPU::MemoryDetector dec = CPU::MemoryDetector(info);
    CPU::MemoryEntry *bestE = dec.FindSuitableEntry();
    u32 ptr_to_free_memory = p.Init(physical_end,bestE->length_of_chunk.top + bestE->base_address.top );
    MemoryManager m = MemoryManager((u32*)ptr_to_free_memory, (u32*)(bestE->base_address.top + bestE->length_of_chunk.top - ptr_to_free_memory));
    CPU::ISR isr;
    __asm__("sti");
    drivers::Screen s;
    s.terminal_clear();
    s.terminal_write_string("Initialize kernel with 2 stage boot sector... Success!\n");
    Util::printf("End of Kernel physic: %x\n",physical_end);
    Util::printf("Setting up paging and heap... Success!\n",physical_end);
    VFS vfs = VFS();
    Util::printf("[D] %s\n", vfs.ReadFile((u8*)"/test.txt"));
    GDT gdt;
    //detection of memory test

    // initialize the isr and the idt for interrupts.


    CPU::Timer timer;
    //we want freq of 50.
    timer.initialize_timer(50);
  //  EnterUserMod();
    while(true);

}
