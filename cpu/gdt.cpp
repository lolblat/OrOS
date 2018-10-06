#include "gdt.h"
tss_entry tss_base_entry;
GDT::GDT()
{
  gdt_entry null_entry;
  Util::memset(&null_entry, sizeof(null_entry), '\x00');

  gdt_entry code_descriptor_kernel;
  code_descriptor_kernel.limit_low = 0xFFFF;
  code_descriptor_kernel.base_low = 0x0;
  code_descriptor_kernel.base_mid = 0x0;
  code_descriptor_kernel.access_byte = 0x9a;
  code_descriptor_kernel.limit_mid = 0xf;
  code_descriptor_kernel.flags = 0xc;
  code_descriptor_kernel.base_high = 0x0;

  gdt_entry data_descriptor_kernel;
  data_descriptor_kernel.limit_low = 0xFFFF;
  data_descriptor_kernel.base_low = 0x0;
  data_descriptor_kernel.base_mid = 0x0;
  data_descriptor_kernel.access_byte = 0x92;
  data_descriptor_kernel.limit_mid = 0xf;
  data_descriptor_kernel.flags = 0xc;
  data_descriptor_kernel.base_high = 0x0;

  gdt_entry code_descriptor_user;
  code_descriptor_user.limit_low = 0xFFFF;
  code_descriptor_user.base_low = 0x0;
  code_descriptor_user.base_mid = 0x0;
  code_descriptor_user.access_byte = 0xfa;
  code_descriptor_user.limit_mid = 0xf;
  code_descriptor_user.flags = 0xc;
  code_descriptor_user.base_high = 0x0;

  gdt_entry data_descriptor_user;
  data_descriptor_user.limit_low = 0xFFFF;
  data_descriptor_user.base_low = 0x0;
  data_descriptor_user.base_mid = 0x0;
  data_descriptor_user.access_byte = 0xf2;
  data_descriptor_user.limit_mid = 0xf;
  data_descriptor_user.flags = 0xc;
  data_descriptor_user.base_high = 0x0;

  u32 base = (u32)&tss_base_entry;
  u32 limit = base + sizeof(tss_base_entry);

  gdt_entry tss_gdt_entry;
  tss_gdt_entry.limit_low = limit & 0xFFFF;
  tss_gdt_entry.base_low = base & 0xFFFF;
  tss_gdt_entry.base_mid = (base >> 16) & 0XFF;
  tss_gdt_entry.access_byte = 0xe9;
  tss_gdt_entry.limit_mid = (limit >> 16) & 0XF;
  tss_gdt_entry.flags = 0xc;
  tss_gdt_entry.base_high = (base >> 24) & 0xFF;
  Util::memset(&tss_base_entry, sizeof(tss_base_entry), '\x00');
  tss_base_entry.ss0 = 0x10;
  tss_base_entry.esp0 = 0xC008F66C; // TODO: need to set up the esp to the kernel frame esp.
  tss_base_entry.cs = 0x1b;
  tss_base_entry.ss = tss_base_entry.ds = tss_base_entry.es = tss_base_entry.fs = tss_base_entry.gs = 0x23;

  gdt_entry entries[] = {
    null_entry,
    code_descriptor_kernel,
    data_descriptor_kernel,
    code_descriptor_user,
    data_descriptor_user,
    tss_gdt_entry
  };
  u8* copy_to = (u8*)MemoryManager::GetInstance()->kmalloc(sizeof(entries));

  Util::memcopy(entries, copy_to, sizeof(entries));
  gdt_lgdt* lgdt = (gdt_lgdt*)MemoryManager::GetInstance()->kmalloc(sizeof(gdt_lgdt));
  lgdt->size = sizeof(entries);
  lgdt->offset = (u32)copy_to;

  asm volatile("lgdt (%0)": :"p" (((u8*)lgdt)));
  asm volatile("mov $0x28,%ax; \
                ltr %ax;");

}
