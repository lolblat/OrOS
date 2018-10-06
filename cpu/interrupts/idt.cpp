//
// Created by lolblat on 14/01/18.
//

//Well, i don't know where to put this comment so i will just put it here.
//There are 3 types of interrupts, 1) exception 2) IRQ 3) software interrupts.


#include "idt.h"
using namespace CPU;
void IDT::set_idt_gate(int n, u32 handler, u8 access)
{
    entries[n].offset_1 = (handler) & (0xFFFF); // high offset of the isr function.
    entries[n].selector = KERNEL_CS; // kernel code.
    entries[n].t_zeros = 0; // padding
    u8 flags_regular = (0b10001110) | (access << 5);
    entries[n].flags = flags_regular; // Bit 1 for use int. 00 - ring 0 - for gate call protection, 0 - to use trap and int, 1110 - for 32 bit int gate
    entries[n].offset_2 = ((handler & 0xFFFF0000) >> 16); // low offset of the isr function.
}

void IDT::set_idt()
{
    pointer.base = (u32)&entries; // we need to load pointer to the int vector.
    pointer.limit = sizeof(idt_entry) * IDT_ENTRIES - 1; // size of this vector.

    //Set the pointer to the idt
    // like GDT in assembly.
    __asm__ __volatile__ ("lidtl (%0)" : : "r" (&pointer));
}
