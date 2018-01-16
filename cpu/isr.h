//
// Created by lolblat on 14/01/18.
//

#ifndef OROS_ISR_H
#define OROS_ISR_H

#include "types.h"

//Extern 32 hardware ints.
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();


//need to push all reg's before doing the int handle.
struct interrupt_frame
{
    u32 ds;
    u32 edi,esi,ebp, esp,ebx,edx,ecx,eax;
    u32 interrupt_number,err_code;
    u32 eip,cs,eflags,useresp,ss;
};
extern "C"
{
    void isr_install();
    void isr_handler(interrupt_frame frame);
}
#endif //OROS_ISR_H
