//
// Created by lolblat on 14/01/18.
//

#ifndef OROS_ISR_H
#define OROS_ISR_H

//some defines about the pic ports.
#include "../types.h"
#include "idt.h"
#include "../../drivers/ports.h"
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
                u8 c1,c2;
                drivers::Ports pic1(PIC1_DATA),pic2(PIC2_DATA),pic1_command(PIC1_COMMAND),pic2_command(PIC2_COMMAND);

                c1 = pic1.port_byte_in();
                c2 = pic2.port_byte_in();


                pic1_command.port_byte_out(ICW1_INIT + ICW1_ICW4);
                pic2_command.port_byte_out(ICW1_INIT + ICW1_ICW4);

                pic1.port_byte_out(0x20);
                pic2.port_byte_out(0x28);

                pic1.port_byte_out(4);
                pic2.port_byte_out(2);

                pic1.port_byte_out(ICW4_8086);
                pic2.port_byte_out(ICW4_8086);

                pic1.port_byte_out(c1);
                pic2.port_byte_out(c2);
            }
            static ISR* m_instance;
        public:
          IDT idt;
          void RegisterIDT(u32 index, u32 function, u8 access)
          {
            idt.set_idt_gate(index, function, access);
          }
            ISR() {
                m_instance = this;
                //create gates for all those functions.
                SetPIC();
                // set idt gates
                RegisterIDT(0, (u32) isr0,0);
                RegisterIDT(1, (u32) isr1,0);
                RegisterIDT(2, (u32) isr2,0);
                RegisterIDT(3, (u32) isr3,3);
                RegisterIDT(4, (u32) isr4,0);
                RegisterIDT(5, (u32) isr5,0);
                RegisterIDT(6, (u32) isr6,0);
                RegisterIDT(7, (u32) isr7,0);
                RegisterIDT(8, (u32) isr8,0);
                RegisterIDT(9, (u32) isr9,0);
                RegisterIDT(10, (u32) isr10,0);
                RegisterIDT(11, (u32) isr11,0);
                RegisterIDT(12, (u32) isr12,0);
                RegisterIDT(13, (u32) isr13,3);
                RegisterIDT(14, (u32) 0,3);
                RegisterIDT(15, (u32) isr15,0);
                RegisterIDT(16, (u32) isr16,0);
                RegisterIDT(17, (u32) isr17,0);
                RegisterIDT(18, (u32) isr18,0);
                RegisterIDT(19, (u32) isr19,0);
                RegisterIDT(20, (u32) isr20,0);
                RegisterIDT(21, (u32) isr21,0);
                RegisterIDT(22, (u32) isr22,0);
                RegisterIDT(23, (u32) isr23,0);
                RegisterIDT(24, (u32) isr24,0);
                RegisterIDT(25, (u32) isr25,0);
                RegisterIDT(26, (u32) isr26,0);
                RegisterIDT(27, (u32) isr27,0);
                RegisterIDT(28, (u32) isr28,0);
                RegisterIDT(29, (u32) isr29,0);
                RegisterIDT(30, (u32) isr30,0);
                RegisterIDT(31, (u32) isr31,0);

                //set irq
                RegisterIDT(IRQ0,(u32)irq0,3);
                RegisterIDT(IRQ1,(u32)irq1,3);
                RegisterIDT(IRQ2,(u32)irq2,3);
                RegisterIDT(IRQ3,(u32)irq3,3);
                RegisterIDT(IRQ4,(u32)irq4,0);
                RegisterIDT(IRQ5,(u32)irq5,0);
                RegisterIDT(IRQ6,(u32)irq6,0);
                RegisterIDT(IRQ7,(u32)irq7,0);
                RegisterIDT(IRQ8,(u32)irq8,0);
                RegisterIDT(IRQ9,(u32)irq9,0);
                RegisterIDT(IRQ10,(u32)irq10,0);
                RegisterIDT(IRQ11,(u32)irq11,0);
                idt.set_idt_gate(IRQ12,(u32)irq12,0);
                idt.set_idt_gate(IRQ13,(u32)irq13,0);
                idt.set_idt_gate(IRQ14,(u32)irq14,3);
                idt.set_idt_gate(IRQ15,(u32)irq15,0);

                //finally set the idt in the cpu.
                idt.set_idt();
            }
            static ISR* GetInstance();
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


    void register_interrupt_handler(u8, u32, u8);
}
#endif //OROS_ISR_H
