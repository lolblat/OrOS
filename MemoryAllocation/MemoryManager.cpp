//
// Created by lolblat on 03/05/18.
//

#include "MemoryManager.h"
MemoryManager::MemoryManager(u32 *start_addr, u32 *end_addr)
{
    m_start = start_addr;
    m_end = end_addr;
    m_allocator.Init(start_addr, end_addr);

    m_page_addr = (u32)m_allocator.AllocatePage(); // last page allocate

   // for(u32* i = m_start; i < m_end; i += 4) // zero the heap
   // {
     //       *i  = 0x0;
    //}


    m_last_free_address = (u32*)m_page_addr;
    *m_last_free_address = PAGE_SIZE;
}

void* MemoryManager::kmalloc(u32 size)
{
    if(size % 12 != 0) // align
    {
        size += 12 - (size % 12);
    }

    while(m_page_addr + PAGE_SIZE <  *m_last_free_address + size + 12) // need to Allocate page
    {
        m_allocator.AllocatePage();
        m_page_addr += PAGE_SIZE;
    }

    u32 free_space_after_alloc = *m_last_free_address - (size + 12);
    *(u32*)(((u8*)m_last_free_address) + NEXT_CHUNK_OFFSET ) = 0x0;
    *(u32*)(((u8*)m_last_free_address) + PREV_CHUNK_OFFSET ) = 0x0;
    *(u32*)(((u8*)m_last_free_address) + SIZE_OF_CHUNK_OFFSET ) = size | CHUNK_IN_USE;
    *(u32*)(((u8*)m_last_free_address) + size + 12 + 4 ) = free_space_after_alloc;
    void *ret = (void*)((u8*)m_last_free_address + 12);
    m_last_free_address = (u32*)((u32)m_last_free_address + size + 12+ 4);

    return ret;
}