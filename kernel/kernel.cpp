//
// Created by lolblat on 10/01/18.
//
extern "C"
{


    #include "../drivers/screen.h"
    #include "../cpu/isr.h"
    #include "../cpu/idt.h"
    #include "../cpu/timer.h"
    //#include "../drivers/keyboard.h"
    #include "../Util/Util.h"
    #include "../cpu/MemoryDetector.h"


    void kernel_main(BootingInfo&);
}

void kernel_main(BootingInfo& info)
{

    drivers::Screen s;
    s.terminal_clear();
    s.terminal_write_string("Initialize kernel with 2 stage boot sector... Success!\n");

    //detection of memory test
    CPU::MemoryDetector dec = CPU::MemoryDetector(info);
    dec.Debug();
    CPU::MemoryEntry* best = dec.FindBestEntry();
    // initialize the isr and the idt for interrupts.
    CPU::ISR isr;
    __asm__("sti");

    CPU::Timer timer;
    //we want freq of 50.
    timer.initialize_timer(50);
    while(true);

}
