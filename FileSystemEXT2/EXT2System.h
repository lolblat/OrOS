//
// Created by lolblat on 07/07/18.
//

#ifndef OROS_EXT2SYSTEM_H
#define OROS_EXT2SYSTEM_H

#endif //OROS_EXT2SYSTEM_H
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


    void ReadSuperBlock();
public:
    EXT2System(ATABus, ATAType);
    u8* ReadBlock(u32 block_number);
    u8* ReadSingleLinked(u32 block_id_for_linked);
    u8* ReadDoubleLinked(u32 block_id_for_linked);
    INode* ReadInode(u32 inode_number);
    u8* GetContentOfINode(INode* inode_number);
    INode* ReadRootINode();
    INode* GetFileByPath(u8* path);
    void ReadDirContent(Directory* entries);
    INode* FindIfFileInDir(Directory* dir, u8* file);
    ~EXT2System();


};