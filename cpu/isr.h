//
// Created by lolblat on 14/01/18.
//

#ifndef OROS_ISR_H
#define OROS_ISR_H

//some defines about the pic ports.
#include "types.h"
#include "idt.h"
#include "../drivers/ports.h"
#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1 + 1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2 + 1)



#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */


#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


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


//extern the irq functions
extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

//need to push all reg's before doing the int handle.
extern "C"
{

    namespace CPU
    {

        class ISR
        {
        private:
            void SetPIC()
            {
                unsigned char c1,c2;
                c1 = drivers::Ports::port_byte_in(PIC1_DATA);
                c2 = drivers::Ports::port_byte_in(PIC2_DATA);

                drivers::Ports::port_byte_out(PIC1_COMMAND, ICW1_INIT + ICW1_ICW4); //INIT
                drivers::Ports::port_byte_out(PIC2_COMMAND, ICW1_INIT + ICW1_ICW4);

                drivers::Ports::port_byte_out(PIC1_DATA, 0x20); // master pic vector offset.
                drivers::Ports::port_byte_out(PIC2_DATA, 0x28); // slave pic vector offset.

                drivers::Ports::port_byte_out(PIC1_DATA,4); // tell master pic that there is a slave at IRQ2
                drivers::Ports::port_byte_out(PIC2_DATA,2);//tell slave pic its cascade identity

                drivers::Ports::port_byte_out(PIC1_DATA,ICW4_8086);
                drivers::Ports::port_byte_out(PIC2_DATA,ICW4_8086);

                drivers::Ports::port_byte_out(PIC1_DATA, c1); //restore mask
                drivers::Ports::port_byte_out(PIC2_DATA, c2);

            }
        public:
            ISR() {
                IDT idt;
                //create gates for all those functions.
                SetPIC();
                // set idt gates
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

                //set irq
                idt.set_idt_gate(IRQ0,(u32)irq0);
                idt.set_idt_gate(IRQ1,(u32)irq1);
                idt.set_idt_gate(IRQ2,(u32)irq2);
                idt.set_idt_gate(IRQ3,(u32)irq3);
                idt.set_idt_gate(IRQ4,(u32)irq4);
                idt.set_idt_gate(IRQ5,(u32)irq5);
                idt.set_idt_gate(IRQ6,(u32)irq6);
                idt.set_idt_gate(IRQ7,(u32)irq7);
                idt.set_idt_gate(IRQ8,(u32)irq8);
                idt.set_idt_gate(IRQ9,(u32)irq9);
                idt.set_idt_gate(IRQ10,(u32)irq10);
                idt.set_idt_gate(IRQ11,(u32)irq11);
                idt.set_idt_gate(IRQ12,(u32)irq12);
                idt.set_idt_gate(IRQ13,(u32)irq13);
                idt.set_idt_gate(IRQ14,(u32)irq14);
                idt.set_idt_gate(IRQ15,(u32)irq15);

                //finally set the idt in the cpu.
                idt.set_idt();
            }
        };
    }

    struct interrupt_frame
    {
        u32 ds;
        u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
        u32 interrupt_number, err_code;
        u32 eip, cs, eflags, useresp, ss;
    };

    void isr_handler(interrupt_frame frame);
    void irq_handler(interrupt_frame frame);
    typedef void (*isr_t)(interrupt_frame);


    void register_interrupt_handler(u8, isr_t);
}
#endif //OROS_ISR_H
