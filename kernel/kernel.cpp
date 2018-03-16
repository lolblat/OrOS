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
    //#include "../drivers/keyboard.h"
    #include "../Util/Util.h"
}

void kernel_main()
{
    drivers::Screen s;
    s.terminal_clear();
    s.terminal_write_string("START\n");
    // initialize the isr and the idt for interrupts.
    CPU::ISR isr;
    __asm__("sti");
    CPU::Timer timer;
    //drivers::Keyboard k;
//    k.keyboard_init();
    //we want freq of 50.
    timer.initialize_timer(50);


}
