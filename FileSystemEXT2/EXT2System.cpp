//
// Created by lolblat on 07/07/18.
//

#include "EXT2System.h"

EXT2System::EXT2System(ATABus bus, ATAType type)
{
    m_hard_disk = (ATA*)MemoryManager::GetInstance()->kmalloc(sizeof(ATA));
    m_hard_disk->SetPorts(bus, type);
    m_hard_disk->ATAIdentify();
    ReadSuperBlock();

}

void EXT2System::ReadSuperBlock()
{
    // we will read 4 blocks for the disk, and then decide what to do next
    u8* super_block_data = m_hard_disk->ATARead(2,2);
    // 2 first blocks, is the boot sector blocks. nothing special there...
    m_super_block = (SuperBlock*)super_block_data;
    if(m_super_block->magic_number != SUPER_BLOCK_MAGIC_NUMBER)
    {
        Util::printf("[E] Invalid magic number\n");
        return ;
    }
    Util::printf("[D] Valid magic number...\n");
    m_size_of_each_block  = 1024 << m_super_block->log10_of_block_size;
    m_sectors_per_block = m_size_of_each_block / ATA_SECTOR_SIZE;
    Util::printf("[D] block size: %d Each block is %d disk sectors\n", m_size_of_each_block, m_sectors_per_block);
    m_total_number_of_blocks = m_super_block->number_of_blocks;
    m_total_number_of_inodes = m_super_block->number_of_inodes;

    m_number_of_blocks_in_block_group = m_super_block->number_of_blocks_in_each_group;
    m_number_of_inodes_in_block_group = m_super_block->number_of_inodes_in_each_group;
    m_total_number_of_block_groups = (int)m_total_number_of_blocks/m_number_of_blocks_in_block_group;
    m_inodes_per_block = m_size_of_each_block / sizeof(INode);
    if(!m_total_number_of_block_groups)
    {
        m_total_number_of_block_groups = 1;
    }
    Util::printf("[D] Number of block groups: %d\n", m_total_number_of_block_groups);
    m_block_group_descriptor_block = m_super_block->block_number_of_superblock + (sizeof(SuperBlock) / m_size_of_each_block);
    Util::printf("[D] Number of block groups: %d block desc at: %d\n", m_total_number_of_block_groups, m_block_group_descriptor_block);
}

INode* EXT2System::ReadInode(u32 inode_number)
{
    u32 block_group_number = (inode_number - 1) / m_number_of_inodes_in_block_group;
    BlockGroupDescriptor* desc = (BlockGroupDescriptor*)ReadBlock(m_block_group_descriptor_block);
    for(u32 i = 0; i < block_group_number; i++)
    {
        desc++;
    }

    u32 index_in_group = (inode_number - 1) % m_number_of_inodes_in_block_group;
    u32 m_block = (index_in_group * sizeof(INode)) / m_size_of_each_block;
    INode* inode = (INode*)ReadBlock(desc->starting_block_address_of_inode_table + m_block);
    u32 inode_index = index_in_group % m_inodes_per_block;
    for(u32 i = 0; i < inode_index; i++)
    {
        inode++;
    }
    return inode;

}

u8* EXT2System::ReadBlock(u32 block_number)
{
    u32 sectors_to_read = m_sectors_per_block;
    if(sectors_to_read == 0)
    {
        sectors_to_read = 1;
    }

    return m_hard_disk->ATARead(sectors_to_read  * block_number, sectors_to_read );

}

INode* EXT2System::ReadRootINode()
{
    INode* root = (INode*)ReadInode(EXT2_ROOT_INODE);
    if((root->type_and_permission&EXT2_TYPE_MASK) != DIRECTORY_FILE_TYPE)
    {
        Util::printf("[E] error, root dir not a dir\n");
        return (INode*)0;
    }
    for(u32 i = 0; i < 12;i++)
    {
        u32 fbp = root->direct_block_ptr[i];
        if(fbp == 0)
        {
            break;
        }
        u8* block = ReadBlock(fbp);
        ReadDirContent((Directory*)block);
    }
    return root;
}

void EXT2System::ReadDirContent(Directory *entries)
{
    while(entries->inode != 0)
    {
        u8* name = (u8*)MemoryManager::GetInstance()->kmalloc(entries->name_length_least_bits);
        Util::memcopy(entries->name_string, name, entries->name_length_least_bits);
        name[entries->name_length_least_bits] = 0;
        Util::printf("[D] found entry: %d %s\n",entries->inode, name);
        entries = (Directory*)((u8*)entries + entries->total_size_of_entry);
    }
}

u8* EXT2System::GetContentOfINode(INode* inode)
{
    u32 first_size = inode->lower_32_bits_size;
    u8* array = (u8*)MemoryManager::GetInstance()->kmalloc(first_size);
    u8* ptr_to_insert = array;
    for(u32 i = 0; i < 12; i ++)
    {
        u32 fast_ptr_block = inode->direct_block_ptr[i];
        if(fast_ptr_block == 0)
        {
            break;
        }
        u8* block = (u8*)ReadBlock(fast_ptr_block);
        Util::memcopy(block, ptr_to_insert, m_size_of_each_block);
        ptr_to_insert += m_size_of_each_block;
    }
    if(inode->block_of_ptr)
    {
        u8* data = ReadSingleLinked(inode->block_of_ptr);
        Util::memcopy(data, ptr_to_insert, (m_size_of_each_block / sizeof(u32)) * m_size_of_each_block);
        ptr_to_insert += (m_size_of_each_block / sizeof(u32)) * m_size_of_each_block;
    }

    if(inode->block_of_blocks_ptr)
    {
        u8* data = ReadDoubleLinked(inode->block_of_blocks_ptr);
        Util::memcopy(data, ptr_to_insert, (m_size_of_each_block / sizeof(u32)) * (m_size_of_each_block / sizeof(u32)) * m_size_of_each_block);
        ptr_to_insert += (m_size_of_each_block / sizeof(u32)) * (m_size_of_each_block / sizeof(u32)) * m_size_of_each_block;
    }
    return array;
}

u8* EXT2System::ReadSingleLinked(u32 block_id_for_linked)
{
    u32 max_blocks_to_link_to = m_size_of_each_block / sizeof(u32);
    u32* block = (u32*)ReadBlock(block_id_for_linked);
    u8* data = (u8*)MemoryManager::GetInstance()->kmalloc(max_blocks_to_link_to * m_size_of_each_block);
    u8* ptr_to_data = data;
    for(u32 i = 0; i < max_blocks_to_link_to; i++)
    {
        if(*block == 0)
        {
            break;
        }
        u8* block_data = ReadBlock(*block);
        Util::memcopy(block_data, ptr_to_data, m_size_of_each_block);
        ptr_to_data += m_size_of_each_block;
    }
    return data;
}

u8* EXT2System::ReadDoubleLinked(u32 block_id_for_linked)
{
    u32 max_blocks_to_link_to = m_size_of_each_block / sizeof(u32);
    u32* block = (u32*)ReadBlock(block_id_for_linked);
    u8* data = (u8*)MemoryManager::GetInstance()->kmalloc(max_blocks_to_link_to * max_blocks_to_link_to * m_size_of_each_block);
    u8* ptr = data;
    for(u32 i = 0; i < max_blocks_to_link_to; i ++)
    {
        if(*block == 0)
        {
            break;
        }
        u8* data = ReadSingleLinked(*block);
        Util::memcopy(data,ptr,max_blocks_to_link_to * m_size_of_each_block);
        ptr += max_blocks_to_link_to * m_size_of_each_block;
    }
    return data;
}

INode* EXT2System::GetFileByPath(u8 *path)
{

    path++;
    Util::strsplit(path,'/');
    INode* root = ReadInode(EXT2_ROOT_INODE);
    INode* rc = (INode*)0;

    while(*path != '\x00')
    {
        Util::printf("[D]%d\n",*path);
        bool found = false;
        for(u32 i = 0; i < 12; i++)
        {
            u32 fbp = root->direct_block_ptr[i];
            if(fbp == 0)
            {
                break;
            }
            u8* block = ReadBlock(fbp);
            rc = FindIfFileInDir((Directory*)block, path);
            if(rc != (INode*)0)
            {
                found = true;
                break;
            }
        }
        if(found)
        {
            Util::printf("[D]%s\n",path);
            path += Util::strlen(path);
        }
        else
        {
            return (INode*)0;
        }
    }
    return rc;

}

INode* EXT2System::FindIfFileInDir(Directory *dir, u8* file)
{
    while(dir->inode != 0)
    {
        u8* n = (u8*)MemoryManager::GetInstance()->kmalloc(dir->name_length_least_bits + 1);
        Util::memcopy(dir->name_string, n, dir->name_length_least_bits);
        n[dir->name_length_least_bits] = '\x00';
        if(n && Util::strcmp(n,file) == 0)
        {
            return ReadInode(dir->inode);
        }
        dir = (Directory*)((u8*)dir + dir->total_size_of_entry);
    }
    return (INode*)0;
}