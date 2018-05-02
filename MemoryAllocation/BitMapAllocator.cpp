//
// Created by lolblat on 16/04/18.
//
#include "BitMapAllocator.h"

BitMapAllocator::BitMapAllocator(u32 start, u32 end)
{
    if(0x10000 - (start&0xFFFF) != 0)
    {
        start += 0x10000 - (start & 0xFFFF);
    }
    m_start_addr = start;
    m_end_addr = end;
    m_next_free_bit.bit = 0;
    m_next_free_bit.index = 0;
    for(u32 i = 0; i < 16318; i++)
    {
        m_bit_map[i] = 0x0;//set all pages to free
    }
}

/*
 * Get page type by page number
 */
PageType BitMapAllocator::GetTypeOfPage(u32 page)
{
    u32 index = page / 8;
    u32 bit = page % 8;
    PageType t = (PageType )(((m_bit_map[index] << bit) & 0x80) >> 7);
    return t;
}


/*
 * Alloc a page and return his physical address
 */
u32 BitMapAllocator::AllocatePage()
{
    u8 bit = 0x1 << m_next_free_bit.bit;
    m_bit_map[m_next_free_bit.index] = m_bit_map[m_next_free_bit.index] | bit;// set as allocated

    u32 index_holder = m_next_free_bit.index;
    u32 bit_holder = m_next_free_bit.bit;

    for(u32 i = 0; i < 16318; i++)
    {
        bool free = false;
        u8 k = 0;
        for(k = 0; k < 8; k++)
        {
            if((PageType)(m_bit_map[i] & (0x1 << k)) == 0) // free page
            {
                free = true;
                break;
            }
        }

        if(free)
        {
            m_next_free_bit.index = i;
            m_next_free_bit.bit = k;
            break;
        }
    }

    index_holder = (index_holder * 4096 * 8); // each index in the bit map table is equal to 8 pages
    bit_holder = (bit_holder * 4096); // each bit is 4096

    u32 physical_page_address = (u32)(m_start_addr + index_holder + bit_holder);
    return physical_page_address;

}

/*
 * Free the given page, change the ptr to look at the page
 */
void BitMapAllocator::Free(u32 page) {
    u32 index = page / 8;
    u8 bit = page % 8;
    m_bit_map[index] = (m_bit_map[index] ^ (0x1
            << bit)); // zero bit map bit, if bit is 1 so 1 ^ 1 = 0 --> free , if page free 0 ^ 0 = 0 --> free to
    m_next_free_bit.index = index;
    m_next_free_bit.bit = bit;
}
