//
// Created by lolblat on 18/03/18.
//

#ifndef OROS_MEMORYDETECTOR_H
#define OROS_MEMORYDETECTOR_H

#include "types.h"
#include "../Util/Util.h"
#include "../drivers/screen.h"
#define MAX_ENTRIES_IN_TABLE 15
extern "C"
{
    namespace CPU
    {

        enum MemoryType
        {
            USABLE_RAM = 1,
            RESERVED = 2,
            ACPI_RECLAIMABLE = 3,
            ACPI_NVS = 4,
            BAD = 5
        };
        struct MemoryEntry
        {
            long_long base_address;
            long_long length_of_chunk;
            MemoryType type;
            u32 aspc;

            void operator=(const MemoryEntry &a)
            {
                base_address = a.base_address;
                length_of_chunk = a.length_of_chunk;
                type = a.type;
                aspc = a.aspc;
            }
        }__attribute__((__packed__));

        class MemoryDetector
        {
            private:
                u32 m_sum_of_entries;
                MemoryEntry m_entries[15];
            public:
                MemoryDetector();
                void Debug();
                MemoryEntry* FindBestEntry();
        };
    }
};
#endif //OROS_MEMORYDETECTOR_H
