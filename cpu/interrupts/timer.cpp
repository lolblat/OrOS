//
// Created by lolblat on 26/01/18.
//

#include "timer.h"
using namespace CPU;

//global tick count var
u32 tick_count = 0;

static void timer_callback(interrupt_frame frame)
{
    //just a tick count
    tick_count++;
}

void Timer::initialize_timer(unsigned int freq)
{
    register_interrupt_handler(IRQ0, (u32)timer_callback,0); // register the IRQ0 function handler, when interrupt from IRQ0 happens, run the timer callback

    u32 divisor = PIC_TIMER_FREQ / freq; // get the freq set.
    u8 low = (u8)(divisor & 0xFF); // find low byte
    u8 high = (u8)((divisor >> 8) & 0xFF); // find high byte

    //set the PIT to get new rate generator with some freq.
    drivers::Ports command(COMMAND_REGISTER),channel_zero(CHANNEL_ZERO);
    command.port_byte_out(CHANNEL_ZERO_OR_KEY | ACCESS_MODE_L_AND_H_OR_KEY | OPERATING_MODE_RATE_GENERATOR);
    // pass freq, first low byte then high byte
    channel_zero.port_byte_out(low);
    channel_zero.port_byte_out(high);
}
