//
// Created by lolblat on 10/01/18.
//
extern "C"
{
    void kernel_main();
    #include "../drivers/screen.h"
    #include "../cpu/isr.h"
    #include "../cpu/idt.h"
}

void kernel_main()
{
    drivers::Screen s;
    s.terminal_clear();

    // initialize the isr and the idt for interrupts.
    CPU::ISR isr;

    //dummy interrupts
    __asm__("int $41");
    __asm__("int $3");
    __asm__("int $4");
}
