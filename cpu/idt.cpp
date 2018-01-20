//
// Created by lolblat on 14/01/18.
//

#include "idt.h"
using namespace CPU;
void IDT::set_idt_gate(int n, u32 handler)
{
    entries[n].offset_1 = (handler) & (0xFFFF);
    entries[n].selector = KERNEL_CS;
    entries[n].t_zeros = 0;
    entries[n].flags = 0b10001110; // Bit 1 for use int. 00 - ring 0 - for gate call protection, 0 - to use trap and int, 1110 - for 32 bit int gate
    entries[n].offset_2 = ((handler & 0xFFFF0000) >> 16);
}

void IDT::set_idt()
{
    pointer.base = (u32)&entries;
    pointer.limit = sizeof(idt_entry) * IDT_ENTRIES - 1;
    //Set the pointer to the idt
    // like GDT in assembly.
    __asm__ __volatile__ ("lidtl (%0)" : : "r" (&pointer));
}
