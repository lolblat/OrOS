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
    #include "../Util/Util.h"
    #include "../cpu/MemoryDetector.h"
#include "../MemoryAllocation/BitMapAllocator.h"
#include "../MemoryAllocation/Paging.h"

    void kernel_main(BootingInfo&,u32,u32);
}

void kernel_main(BootingInfo& info, u32 physical_end, u32 virtual_end)
{

    drivers::Screen s;
    Paging p = Paging();

    s.terminal_clear();
    s.terminal_write_string("Initialize kernel with 2 stage boot sector... Success!\n");

    //detection of memory test
    CPU::MemoryDetector dec = CPU::MemoryDetector(info);
    dec.Debug();
    Util::printf("End of Kernel physic: %x\n",physical_end);
    CPU::MemoryEntry *bestE = dec.FindSuitableEntry();

    u32 free_memory = p.Init(physical_end,bestE->length_of_chunk.top + bestE->base_address.top );

    BitMapAllocator alloc = BitMapAllocator(free_memory, bestE->length_of_chunk.top + bestE->base_address.top);
    Util::printf("[D] %x %x %x %x\n",alloc.AllocatePage(),alloc.AllocatePage(),alloc.AllocatePage(),alloc.AllocatePage());
     // initialize the isr and the idt for interrupts.

    CPU::ISR isr;
    __asm__("sti");

    CPU::Timer timer;
    //we want freq of 50.
    timer.initialize_timer(50);
    while(true);

}
