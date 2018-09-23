//
// Created by lolblat on 27/04/18.
//

#ifndef OROS_PAGING_H
#define OROS_PAGING_H

#include "../cpu/types.h"
#include "../Util/Util.h"
#define PAGE_FRAME_OF_KERNEL 768
#define PAGE_TABLE_OPTION 0x7
#define PAGE_FRAME_OPTION 0X7
class Paging
{

private:
    u32* m_page_dir;
    u32  m_page_table_index;
    u32  m_page_frame_index;
public:
    Paging();
    u32 Init(u32 start,u32 end);

};
#endif //OROS_PAGING_H
