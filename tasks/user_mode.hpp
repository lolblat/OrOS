#ifndef _USER_MODE
#define _USER_MODE
#include "../cpu/types.h"
struct tss_entry
{
  u32 prevTss;
  	u32 esp0;
  	u32 ss0;
  	u32 esp1;
  	u32 ss1;
  	u32 esp2;
  	u32 ss2;
  	u32 cr3;
  	u32 eip;
  	u32 eflags;
  	u32 eax;
  	u32 ecx;
  	u32 edx;
  	u32 ebx;
  	u32 esp;
  	u32 ebp;
  	u32 esi;
  	u32 edi;
  	u32 es;
  	u32 cs;
  	u32 ss;
  	u32 ds;
  	u32 fs;
  	u32 gs;
  	u32 ldt;
  	u16 trap;
  	u16 iomap;
}__attribute__((packed));

void EnterUserMod()
{
  __asm__ volatile (" \
   cli; \
   mov $0x23, %ax; \
   mov %ax, %ds; \
   mov %ax, %es; \
   mov %ax, %fs; \
   mov %ax, %gs; \
                 \
   mov %esp, %eax; \
   pushl $0x23; \
   pushl %eax; \
   pop %eax ; \
   push %eax ; \
   pushf; \
   pushl $0x1B; \
   push $1f; \
   iret; \
 1: \
  ");
}
#endif
