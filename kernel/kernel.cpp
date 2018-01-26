//
// Created by lolblat on 10/01/18.
//
extern "C"
{
    void kernel_main();
    #include "../drivers/screen.h"
    #include "../cpu/isr.h"
    #include "../cpu/idt.h"
    #include "../cpu/timer.h"
}

void kernel_main()
{
    drivers::Screen s;
    s.terminal_clear();
    // initialize the isr and the idt for interrupts.
    CPU::ISR isr;
    CPU::Timer timer;

    //we want freq of 50.
    timer.initialize_timer(50);

    //dummy interrupts

    // to enable interrupts, to get timer ticks and such
    __asm__("sti");
    //__asm__("int $32");
    //__asm__("int $3");
    //__asm__("int $4");

}
