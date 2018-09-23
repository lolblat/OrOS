#ifndef _GDT
#define _GDT
#include "types.h"
#include "../Util/Util.h"
#include "../tasks/user_mode.hpp"
#include "../MemoryAllocation/MemoryManager.h"

struct gdt_entry
{
  u16 limit_low;
  u16 base_low;
  u8 base_mid;
  u8 access_byte;
  u8 limit_mid:4;
  u8 flags:4;
  u8 base_high;
}__attribute__((packed));

struct gdt_lgdt
{
  u16 size;
  u32 offset;
}__attribute__((packed));

class GDT
{
private:
public:
  GDT();
};

#endif
