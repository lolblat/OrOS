//
// Created by lolblat on 07/07/18.
//

#ifndef OROS_EXT2SYSTEM_H
#define OROS_EXT2SYSTEM_H

#define EXT2_ROOT_INODE 2
#include "FileSystemStructs.h"
#include "../drivers/ATA.h"


// defines an ext2 file system on specific ATA hard disk.
class EXT2System
{
private:
    SuperBlock *m_super_block; // save the super block of the system
    BlockGroupDescriptor *m_block_group_descriptor;
    ATA *m_hard_disk;

    u32 m_size_of_each_block;
    u32 m_total_number_of_inodes;
    u32 m_total_number_of_blocks;
    u32 m_number_of_blocks_in_block_group;
    u32 m_number_of_inodes_in_block_group;
    u32 m_inodes_per_block;
    u32 m_total_number_of_block_groups;
    u32 m_sectors_per_block;

    u32 m_block_group_descriptor_block;

    u32 AllocateBlock();
    u32 AllocateINode();
    void ReadSuperBlock();
public:
    EXT2System(ATABus, ATAType);
    u8* ReadBlock(u32 block_number);
    void WriteBlock(u8* data, u32 block_number, u32 size);
    u8* ReadSingleLinked(u32 block_id_for_linked);
    u8* ReadDoubleLinked(u32 block_id_for_linked);
    INode* ReadInode(u32 inode_number);
    u8* GetContentOfINode(INode* inode_number);
    INode* ReadRootINode();
    INode* GetFileByPath(u8* path);
    void ReadDirContent(Directory* entries);
    INode* FindIfFileInDir(Directory* dir, u8* file);
    void AppendToDir(INode* d, u32 inode_to_append, u8* path_to_append_to, u8 file_type);
    bool AppendEntry(u32 block_number_of_dir ,Directory* add_entry);
    void WriteToINode(INode* data, u32 inode_number, u32 size);
    void WriteToFile(u8* file_name, u8* data, u32 size);
    ~EXT2System();


};

#endif //OROS_EXT2SYSTEM_H
