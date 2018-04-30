//
// Created by lolblat on 14/01/18.
//

#ifndef OROS_IDT_H
#define OROS_IDT_H

#include "../types.h"
#define  KERNEL_CS  0x08
#define IDT_ENTRIES 256
extern "C"
{
    namespace CPU {
        class IDT {
        public:
            struct idt_entry {
                u16 offset_1; // offset bits 0..15
                u16 selector; // a code segment selector GDT or LDT
                u8 t_zeros; // unused set to zeros
                u8 flags; // flags
                /*
                 * Bit 7: "interrupt is present"
                 * Bit 6-5: ring (0 = kernel ... 3 = user)
                 * Bit 4: set to 0 for int gates
                 * Bit 3-0: 1110 = "32 bit int gate"
                 */
                u16 offset_2;// offset bits 16..31
            }__attribute__((packed));

            struct idt_pointer {
                u16 limit; // length of the idt.
                u32 base; // linear address of the idt, where INT 0 is.
            }__attribute__((packed));

            void set_idt_gate(int n, u32 handler);

            void set_idt();

        private:
            idt_entry entries[IDT_ENTRIES]; // for 256 int's
            idt_pointer pointer; // for holding a pointer and load it into the idt.
        };
    }
};

#endif //OROS_IDT_H
