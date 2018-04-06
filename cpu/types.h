//
// Created by lolblat on 14/01/18.
//

#ifndef OROS_TYPES_H
#define OROS_TYPES_H
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;

struct long_long
{
    u32 top;
    u32 bottom;
    void operator=(const long_long& a)
    {
        top = a.top;
        bottom = a.bottom;
    }
};
struct BootingInfo
{
    u32 ptr_to_memory_map;
    u32 count_of_memory_entries;
};
#endif //OROS_TYPES_H
