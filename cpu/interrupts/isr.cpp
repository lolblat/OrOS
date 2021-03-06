//
// Created by lolblat on 14/01/18.
//

#include "isr.h"
#include "../../Util/Util.h"
#include "../../drivers/screen.h"
using namespace CPU;
ISR* ISR::m_instance = (ISR*)0;

u32 interrupts_handler[256]; // all interrupts.


void isr_handler(interrupt_frame frame)
{
    // isr handler function - print the detail about the int.
    drivers::Screen s;
    s.terminal_write_string("[D] received CPU ISR interrupt");
    s.terminal_write_string("\n");
}

void irq_handler(interrupt_frame frame)
{
    drivers::Screen s;
    u32 int_num = frame.interrupt_number; // get interrupt number
    //send EOI to the pic
    if(int_num >= 32 /*irq first interrupt*/ + 8/*for slave*/)
    {
        drivers::Ports p(0xA0);
        p.port_byte_out(0x20);
    }
    drivers::Ports p(0x20);
    p.port_byte_out(0x20);
    //start interrupt handler
    if(int_num == 0x21)
    {
      Util::printf("[D] here\n");
    }
    if(interrupts_handler[int_num] != 0)
    {

      isr_t handler = (isr_t)interrupts_handler[int_num]; // get handle address
      handler(frame); //start handle.
    }
}


//register new interrupt handler.
void register_interrupt_handler(u8 offset, u32 handler ,u8 access)
{
    //set interrupt handler at offset.
    interrupts_handler[offset] = handler;
    ISR::GetInstance()->RegisterIDT(offset, handler, access);

}

ISR* ISR::GetInstance()
{
    return m_instance;
}
