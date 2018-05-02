//
// Created by lolblat on 27/04/18.
//

#include "Paging.h"

Paging::Paging()
{
    u32 cr3_value;
    __asm__ __volatile__ (
    "mov %%cr3, %%eax\n\t"
            "mov %%eax, %0\n\t"

    : "=m" (cr3_value)
    : /* no input */
    : "%eax"
    );

    m_page_dir = (u32 *) cr3_value;
    m_page_frame_index = 0;
    m_page_table_index = 0;

}

u32 Paging::Init(u32 free_memory_start_address, u32 free_memory_end_address) // allocate the pages on the free memory
{

    if(0x10000 - (free_memory_start_address&0xFFFF) != 0) // align the address
    {
        free_memory_start_address += 0x10000 - (free_memory_start_address & 0xFFFF);
    }
    m_page_table_index = free_memory_start_address >> 22;

    bool alloc_new_table = true;

    m_page_frame_index = free_memory_start_address >> 12 & 0x3FF;
    u32 ptr_to_free_memory_for_storage = free_memory_start_address;

    if(m_page_table_index == 0)
    {
        m_page_table_index++;
        m_page_frame_index = 0;
        free_memory_start_address = 0x400000;
    }

    while(free_memory_start_address <= free_memory_end_address && m_page_table_index != PAGE_FRAME_OF_KERNEL)
    {

        u32 *page_dir_table_index = (u32*)((u32)m_page_dir + m_page_table_index * 4 + 0xC0000000);

        u32 *page_frames = (u32*)(ptr_to_free_memory_for_storage + 0xC0000000);

        for(u32 i = 0; i < 1024; i++)
        {
            u32 addr = (u32)((free_memory_start_address + i * 4096)| 1);
            page_frames[i] = addr;

        }


        *page_dir_table_index = (((u32)page_frames - 0xC0000000)| PAGE_TABLE_OPTION);
        m_page_table_index++;
        free_memory_start_address += 1024 * 4096;
        ptr_to_free_memory_for_storage += 1024 * 4;

    }
    return ptr_to_free_memory_for_storage;
}