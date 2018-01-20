//
// Created by lolblat on 14/01/18.
//

#include "isr.h"
#include "../drivers/screen.h"
using namespace CPU;

void isr_handler(interrupt_frame frame)
{
    drivers::Screen s;
    s.terminal_write_string("[D] received int");
    s.terminal_write_string("\n");
}