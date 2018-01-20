//
// Created by lolblat on 14/01/18.
//

#ifndef OROS_ISR_H
#define OROS_ISR_H

#include "types.h"
#include "idt.h"
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
extern "C"
{
    namespace CPU {
        struct interrupt_frame {
            u32 ds;
            u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
            u32 interrupt_number, err_code;
            u32 eip, cs, eflags, useresp, ss;
        };

        class ISR {
        public:
            ISR() {
                IDT idt;
                idt.set_idt_gate(0, (u32) isr0);
                idt.set_idt_gate(1, (u32) isr1);
                idt.set_idt_gate(2, (u32) isr2);
                idt.set_idt_gate(3, (u32) isr3);
                idt.set_idt_gate(4, (u32) isr4);
                idt.set_idt_gate(5, (u32) isr5);
                idt.set_idt_gate(6, (u32) isr6);
                idt.set_idt_gate(7, (u32) isr7);
                idt.set_idt_gate(8, (u32) isr8);
                idt.set_idt_gate(9, (u32) isr9);
                idt.set_idt_gate(10, (u32) isr10);
                idt.set_idt_gate(11, (u32) isr11);
                idt.set_idt_gate(12, (u32) isr12);
                idt.set_idt_gate(13, (u32) isr13);
                idt.set_idt_gate(14, (u32) isr14);
                idt.set_idt_gate(15, (u32) isr15);
                idt.set_idt_gate(16, (u32) isr16);
                idt.set_idt_gate(17, (u32) isr17);
                idt.set_idt_gate(18, (u32) isr18);
                idt.set_idt_gate(19, (u32) isr19);
                idt.set_idt_gate(20, (u32) isr20);
                idt.set_idt_gate(21, (u32) isr21);
                idt.set_idt_gate(22, (u32) isr22);
                idt.set_idt_gate(23, (u32) isr23);
                idt.set_idt_gate(24, (u32) isr24);
                idt.set_idt_gate(25, (u32) isr25);
                idt.set_idt_gate(26, (u32) isr26);
                idt.set_idt_gate(27, (u32) isr27);
                idt.set_idt_gate(28, (u32) isr28);
                idt.set_idt_gate(29, (u32) isr29);
                idt.set_idt_gate(30, (u32) isr30);
                idt.set_idt_gate(31, (u32) isr31);
                idt.set_idt();
            }
        };
    }
    void isr_handler(CPU::interrupt_frame frame);
}
#endif //OROS_ISR_H
