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
    terminal_clear();
    isr_install();
    __asm__("int $2");
    __asm__("int $3");
    __asm__("int $4");
}
