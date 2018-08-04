//
// Created by lolblat on 10/05/18.
//

#ifndef OROS_INODE_H
#define OROS_INODE_H
// All this OS is designed in OOP, but this will be struct
#include "../cpu/types.h"


//file types defines
#define FIFO_FILE_TYPE 0x1000
#define CHARACTER_DEVICE_FILE_TYPE 0x2000
#define DIRECTORY_FILE_TYPE 0x4000
#define BLOCK_DEVICE_FILE_TYPE 0x6000
#define REGULAR_FILE_FILE_TYPE 0x8000
#define SYMBOLIC_LINK_FILE_TYPE 0xA000
#define UNIX_SOCKET_FILE_TYPE 0xC000
#define INODE_DIRECT_POINTERS_LENGTH 12
#define EXT2_TYPE_MASK 0xF000



#define DIR_MAX_NAME_LENGTH 255
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

#define SUPER_BLOCK_MAGIC_NUMBER 0xEF53

struct SuperBlock
{
    u32 number_of_inodes;
    u32 number_of_blocks;
    u32 number_of_blocks_reserved_for_root;
    u32 number_of_unallocated_blocks;
    u32 number_of_unallocated_inodes;
    u32 block_number_of_superblock;
    u32 log10_of_block_size;
    u32 log10_of_fragment_size;
    u32 number_of_blocks_in_each_group;
    u32 number_of_fragments_in_each_group;
    u32 number_of_inodes_in_each_group;
    u32 last_mount_time;
    u32 last_written_time;
    u16 mount_times;
    u16 max_mount;
    u16 magic_number;
    u16 file_system_state;
    u16 error_handler;
    u16 minor_version;
    u32 last_check;
    u32 intervals_between_checks;
    u32 creator_os;
    u32 major_version;
    u16 user_id_for_blocks;
    u16 group_id_for_blocks;
    u8 padd[940];
}__attribute__((packed));

struct BlockGroupDescriptor
{
    u32 block_address_for_block_usage_bitmap;
    u32 block_address_for_inode_usage_bitmap;
    u32 starting_block_address_of_inode_table;
    u16 number_of_unallocated_blocks_in_group;
    u16 number_of_unallocated_inodes_in_group;
    u16 number_of_dirs_in_group;
    u32 big_padding[3];
    u16 small_padding;
}__attribute__((packed));

struct INode
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
    u32 disk_sectors_in_use;
    u32 flags;
    u32 op_spec_1;

    //12 fast ptr
    u32 direct_block_ptr[INODE_DIRECT_POINTERS_LENGTH];
    u32 block_of_ptr; // ptr to block that is a list of 1000 ptr's to data
    u32 block_of_blocks_ptr;  // ptr to block of blocks
    u32 block_of_blocks_of_blocks_ptr; // ptr to block of blocks of blocks


    u32 gen_number;

    u32 padd1;
    u32 padd2;

    u32 block_frag_address;

    u32 padd3;
    u32 padd4;
    u32 padd5;


}__attribute__((packed));

struct Directory
{
    u32 inode;
    u16 total_size_of_entry;
    u8 name_length_least_bits;
    u8 type_indicator;
    u8 name_string[DIR_MAX_NAME_LENGTH];
}__attribute__((packed));
#define DIRECTORY_PADDING sizeof(u32) + sizeof(u16) + sizeof(u8) + sizeof(u8)

class NameListNode
{
private:
    u8* name;
    NameListNode* next;
public:
    NameListNode(u8* name)
    {
        this->name = name;
        this->next = (NameListNode*)0;
    }
    void AddNext(NameListNode* name)
    {
        next = name;
    }
    u8* GetName()
    {
        return name;
    }
    NameListNode* GetNext()
    {
        return next;
    }

};

class NameList
{
private:
    NameListNode* last;
    NameListNode* start;
public:
    NameList()
    {
        start = (NameListNode*)0;
        last = (NameListNode*)0;
    }
    void AddNodeList(NameListNode* list_node)
    {
        if(start == (NameListNode*)0 && (NameListNode*)last == 0) // first
        {
            start = list_node;
            last = start;
        }
        else
        {
            last->AddNext(list_node);
            last = list_node;
        }
    }

    u32 GetSize()
    {
        NameListNode* p = start;
        u32 sum = 0;
        while(p)
        {
            sum++;
            p = p->GetNext();
        }
        return sum;
    }

    u8* GetNameAt(u32 i)
    {
        if(i >= GetSize())
        {
            return (u8*)0;
        }
        NameListNode* ptr = start;
        for(u32 k = 0; k < i; k++)
        {
            ptr = ptr->GetNext();
        }

        return ptr->GetName();
    }

};

#endif //OROS_INODE_H
