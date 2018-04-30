//
// Created by lolblat on 18/03/18.
//

#include "MemoryDetector.h"
CPU::MemoryDetector::MemoryDetector(BootingInfo& info)
{
    u64 sum_of_entries = 0;
    sum_of_entries = info.count_of_memory_entries;
    m_sum_of_entries = sum_of_entries;
    Util::printf("There are: %d entries in the memory map\n", m_sum_of_entries);
    char* memory_map_base_address = ((char*)info.ptr_to_memory_map + 0xC0000000);
    for(u32 i = 0; i < sum_of_entries && i < 15; i++)
    {
        MemoryEntry tmp;
        Util::memread((void*)(memory_map_base_address + i * sizeof(MemoryEntry)), &tmp, sizeof(MemoryEntry),1); // + acsp padding
        m_entries[i].base_address = tmp.base_address;
        m_entries[i].length_of_chunk = tmp.length_of_chunk;
        m_entries[i].type = tmp.type;
    }
}

void CPU::MemoryDetector::Debug()
{
    drivers::Screen s;
    s.terminal_write_string("------Memory Detection ------\n");
    s.terminal_write_string("  Base address   |     Length      | Type\n");
    for(u32 i = 0; i < m_sum_of_entries; i++)
    {
        Util::printf("%x|%x|  %d\n",m_entries[i].base_address.top,m_entries[i].length_of_chunk.top,m_entries[i].type);
    }

}

CPU::MemoryEntry* CPU::MemoryDetector::FindSuitableEntry()
{
    MemoryEntry* ptr = 0x00;
    for(u32 i = 0; i < m_sum_of_entries; i++)
    {
        if(m_entries[i].type == USABLE_RAM && m_entries[i].base_address.top > 0x9F000) // some kernel address, like rly basic address for kernel and such, need to decide to map the kernel or not and than use that
        {
            ptr = &(m_entries[i]);
        }
    }
    return ptr;
}