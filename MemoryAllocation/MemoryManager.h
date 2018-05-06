//
// Created by lolblat on 03/05/18.
//

#ifndef OROS_MEMORYMANAGER_H
#define OROS_MEMORYMANAGER_H


extern "C"
{
    #include "../cpu/types.h"

    #include "BitMapAllocator.h"

    #define SIZE_OF_CHUNK_OFFSET 0
    #define NEXT_CHUNK_OFFSET 4
    #define PREV_CHUNK_OFFSET 8
    #define KMALLOC_HEADER_LENGTH 12
    #define PAGE_SIZE 4096
    #define CHUNK_IN_USE 0x1
    #define CHUNK_NOT_IN_USE 0x0

    class MemoryManager
    {
        private:
            u32 *m_start,*m_end;
            u32 *m_last_free_address;
            u32 m_page_addr;
            BitMapAllocator m_allocator;
        public:
            MemoryManager(u32 *start_addr, u32 *end_addr);
            void *kmalloc(u32 size);
            void free(u32 *addr);

    };
};
#endif //OROS_MEMORYMANAGER_H
