//
// Created by lolblat on 16/04/18.
//

#ifndef OROS_BITMAP_H
#define OROS_BITMAP_H
#include "../cpu/types.h"
#include "../Util/Util.h"
enum PageType
{
    FreePage,
    AllocatedPage
};

/*
 * Class the represent the bitmap that save the memory state
 */
class BitMapAllocator
{
private:

    u32 m_start_addr;
    u32 m_end_addr;

    u8 m_bit_map[16318];
    struct
    {
        u32 index;
        u8 bit;
    }m_next_free_bit;

public:
    BitMapAllocator(u32 start, u32 end);
    PageType GetTypeOfPage(u32 page);
    u32 AllocatePage();
    void Free(u32 page);
};
#endif //OROS_BITMAP_H
