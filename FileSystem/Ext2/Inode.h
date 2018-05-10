//
// Created by lolblat on 10/05/18.
//

#ifndef OROS_INODE_H
#define OROS_INODE_H
// All this OS is designed in OOP, but this will be struct
#include "../../cpu/types.h"


//file types defines
#define FIFO_FILE_TYPE 0x1000
#define CHARACTER_DEVICE_FILE_TYPE 0x2000
#define DIRECTORY_FILE_TYPE 0x4000
#define BLOCK_DEVICE_FILE_TYPE 0x6000
#define REGULAR_FILE_FILE_TYPE 0x8000
#define SYMBOLIC_LINK_FILE_TYPE 0xA000
#define UNIX_SOCKET_FILE_TYPE 0xC000

//permission defines
#define OTHER_EXECUTE 0x001
#define OTHER_WRITE 0x002
#define OTHER_READ 0x004
#define GROUP_EXECUTE 0x008
#define GROUP_WRITE 0x010
#define GROUP_READ 0x020
#define USER_EXECUTE 0x040
#define USER_WRITE 0x080
#define USER_READ 0x100
#define STICKY_BIT 0x200 // dir permission.
#define SET_GROUP_ID 0x400
#define SET_USER_ID 0x800




struct Inode
{
    u16 type_and_permission;
    u16 user_id;
    u32 lower_32_bits_size;
    u32 last_access_time;
    u32 creation_time;
    u32 last_modification_time;
    u32 deletion_time;
    u16 group_id;
    u16 hard_link_count;
    u16 disk_sectors_in_use;
    u32 flags;
    u32 op_spec_1;

    //12 fast ptr
    u32* direct_block_ptr_0;
    u32* direct_block_ptr_1;
    u32* direct_block_ptr_2;
    u32* direct_block_ptr_3;
    u32* direct_block_ptr_4;
    u32* direct_block_ptr_5;
    u32* direct_block_ptr_6;
    u32* direct_block_ptr_7;
    u32* direct_block_ptr_8;
    u32* direct_block_ptr_9;
    u32* direct_block_ptr_10;
    u32* direct_block_ptr_11;

    u32* block_of_ptr; // ptr to block that is a list of 1000 ptr's to data
    u32* block_of_blocks_ptr;  // ptr to block of blocks
    u32* block_of_blocks_of_blocks_ptr; // ptr to block of blocks of blocks


    u32 gen_number;

    u32 padd1;
    u32 padd2;

    u32 block_frag_address;

    u32 padd3;
    u32 padd4;
    u32 padd5;


};


#endif //OROS_INODE_H
