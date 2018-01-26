//
// Created by lolblat on 26/01/18.
//

#ifndef OROS_TIMER_H
#define OROS_TIMER_H

#include "../drivers/ports.h"
#include "../drivers/screen.h"
#include "isr.h"
#include "types.h"

#define PIC_TIMER_FREQ 1193180
#define COMMAND_REGISTER 0x43
#define CHANNEL_ZERO 0x40

#define CHANNEL_ZERO_OR_KEY 0
#define ACCESS_MODE_L_AND_H_OR_KEY 0b00110000
#define OPERATING_MODE_RATE_GENERATOR 0b110

extern "C"
{
    namespace CPU
    {
        class Timer {
        public:
            void initialize_timer(unsigned int freq);
        };
    }

    static void timer_callback(interrupt_frame frame);
}

#endif //OROS_TIMER_H
