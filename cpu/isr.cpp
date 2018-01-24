//
// Created by lolblat on 14/01/18.
//

#include "isr.h"
#include "../drivers/screen.h"
using namespace CPU;


isr_t interrupts_handler[256]; // all interrupts.
void register_interrupt_handler(u8 offset, isr_t handler);

void isr_handler(interrupt_frame frame)
{

    // isr handler function - print the detail about the int.
    drivers::Screen s;
    s.terminal_write_string("[D] received int");
    s.terminal_write_string("\n");
}

extern "C" void irq_handler(interrupt_frame frame)
{
    drivers::Screen s;

    s.terminal_write_string("[D] got IRQ interrupt");
    s.terminal_write_string("\n");

    u32 int_num = frame.interrupt_number; // get interrupt number
    //send EOI to the pic
    if(int_num >= 32 /*irq first interrupt*/ + 8/*for slave*/)
    {
        drivers::Ports::port_byte_out(0xA0,0x20);
    }
    drivers::Ports::port_byte_out(0x20,0x20);

    //start interrupt handler
    if(interrupts_handler[int_num] != 0)
    {
        isr_t handler = interrupts_handler[int_num]; // get handle address
        handler(frame); //start handle.
    }
}
void register_interrupt_handler(u8 offset, isr_t handler)
{
    interrupts_handler[offset] = handler;
}