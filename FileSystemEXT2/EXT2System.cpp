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

void EXT2System::WriteBlock(u8 *data, u32 block_number, u32 size)
{
    u32 sectors = m_sectors_per_block;
    if(sectors == 0)
    {
        sectors = 1;
    }
    m_hard_disk->ATAWriteSectors(data, sectors * block_number, size);
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
        Util::printf("[D] found entry: %d %s %d type: %d\n",entries->inode, name, entries->total_size_of_entry, entries->type_indicator);
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
        Util::printf("%d\n",fast_ptr_block);
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

u32 EXT2System::AllocateBlock()
{
    BlockGroupDescriptor* desc = (BlockGroupDescriptor*)ReadBlock(m_block_group_descriptor_block);
    for(u32 i = 0; i < m_total_number_of_block_groups; i++)
    {
        if(desc->number_of_unallocated_blocks_in_group)
        {
            u32 block_number = m_super_block->number_of_blocks_in_each_group - desc->number_of_unallocated_blocks_in_group + 1;
            desc->number_of_unallocated_blocks_in_group--;
            WriteBlock((u8*)desc, m_block_group_descriptor_block + i, m_size_of_each_block);

            m_super_block->number_of_unallocated_blocks--;
            WriteBlock((u8*)m_super_block, m_super_block->block_number_of_superblock, m_size_of_each_block);
            return block_number;
        }
        desc++;
    }
    return -1;
}

u32 EXT2System::AllocateINode()
{
    BlockGroupDescriptor* desc = (BlockGroupDescriptor*)ReadBlock(m_block_group_descriptor_block);
    for(u32 i = 0; i < m_total_number_of_block_groups; i++)
    {
        if(desc->number_of_unallocated_inodes_in_group)
        {
            u32 inode_number = ((i + 1) * m_super_block->number_of_inodes_in_each_group) - desc->number_of_unallocated_inodes_in_group + 1;
            // the (i+1) mul is for the inode indexing system, inode starts from 1...n
            desc->number_of_unallocated_inodes_in_group--;
            WriteBlock((u8*)desc, m_block_group_descriptor_block + i, m_size_of_each_block);

            m_super_block->number_of_unallocated_inodes--;
            WriteBlock((u8*)m_super_block, m_super_block->block_number_of_superblock, m_size_of_each_block);
            return inode_number;
        }
        desc++;
    }
    return -1;
}

void EXT2System::WriteToINode(INode *data, u32 inode_number, u32 size)
{
    u32 block_group_number = (inode_number - 1) / m_number_of_inodes_in_block_group;
    BlockGroupDescriptor* desc = (BlockGroupDescriptor*)ReadBlock(m_block_group_descriptor_block);
    for(u32 i = 0; i < block_group_number; i++)
    {
        desc++;
    }

    u32 index_in_group = (inode_number - 1) % m_number_of_inodes_in_block_group;
    u32 m_block = (index_in_group * sizeof(INode)) / m_size_of_each_block;
    u32 final_index = desc->starting_block_address_of_inode_table + m_block;
    INode* inode = (INode*)ReadBlock(final_index);
    index_in_group = index_in_group % m_inodes_per_block;
    for(u32 i = 0;  i < index_in_group; i++)
    {
        inode ++;
    }

    // found inode
    Util::memcopy((void*)data, (void*)inode, sizeof(INode));
    WriteBlock((u8*)inode, final_index, sizeof(INode));
}

void EXT2System::WriteToFile(u8 *file_name, u8 *data, u32 size)
{
    INode* file_inode = GetFileByPath(file_name);
    if(file_inode == (INode*)0) // no such file, need to create the file.
    {
        u32 inode_number = AllocateINode();
        Util::printf("Node number: %d\n",inode_number);
        INode* inode = (INode*)MemoryManager::GetInstance()->kmalloc(sizeof(INode));
        Util::memset(inode, sizeof(INode), '\x00');
        u32 blocks_for_inode = size / m_size_of_each_block;
        blocks_for_inode++; // need at least one, and its better one more block the lose data in the division.
        if(blocks_for_inode > 12)
        {
            Util::printf("[D] need to implement\n");
            return;
        }

        for(u32 i = 0; i < blocks_for_inode; i++)
        {
            u32 block_number = AllocateBlock();
            Util::printf("Allocated block: %d\n",block_number);
            inode->direct_block_ptr[i] = block_number;
        }
        inode->lower_32_bits_size = size;
        WriteToINode(inode, inode_number, sizeof(INode));

        for(u32 i = 0; i < blocks_for_inode; i++)
        {
            u8* block = ReadBlock(inode->direct_block_ptr[i]);
            u32 size_written = m_size_of_each_block;

            if(i +1 < blocks_for_inode)
            {
                Util::memcopy(data + i * m_size_of_each_block, block, size_written);
            }
            else // last block
            {
                size_written = size;
                Util::memcopy(data + i*m_size_of_each_block, block, size_written);
            }
            WriteBlock(block, inode->direct_block_ptr[i], size);
            size -= m_size_of_each_block;
        }
        Util::printf("[D] added file to table\n");
        AppendToDir(ReadRootINode(),inode_number, file_name, 1);
        return;
    }
    else
    {
        // we will overwrite the data;
        // first we need to check if we need more blocks for the file.
        u32 number_of_blocks_in_inode = file_inode->lower_32_bits_size / m_size_of_each_block;
        u32 number_of_blocks_needed = size / m_size_of_each_block;
        u32 final_number_of_blocks = 0;
        if(number_of_blocks_needed > number_of_blocks_in_inode)
        {

            for(u32 i = number_of_blocks_in_inode; i < number_of_blocks_needed; i++)
            {
                u32 block_number = AllocateBlock();
                file_inode->direct_block_ptr[i] = block_number;
            }
            final_number_of_blocks = number_of_blocks_needed;
        }
        else
        {
            final_number_of_blocks = number_of_blocks_in_inode;
        }
        file_inode->lower_32_bits_size = size;
        for(u32 i = 0; i < final_number_of_blocks; i++)
        {
            u8* block_desc = ReadBlock(file_inode->direct_block_ptr[i]);
            if(size == 0 ) // just fill with null's
            {
                Util::memset(block_desc,m_size_of_each_block, '\x00');

            }
            else if(size < m_size_of_each_block)
            {
                Util::memcopy(data + i * m_size_of_each_block, block_desc, size);
                Util::memset(block_desc + size, m_size_of_each_block - size, '\x00');
            }
            else
            {
                Util::memcopy(data + i* m_size_of_each_block,block_desc,m_size_of_each_block);
            }
            WriteBlock(block_desc, file_inode->direct_block_ptr[i], m_size_of_each_block);
            size -= m_size_of_each_block;
        }

        //TODO: need to write write back to the ata driver of the "new" inode
        return;
    }
}

void EXT2System::AppendToDir(INode* d, u32 inode_to_append, u8* path_to_append_to, u8 file_type)
{
  //INode* root_inode = ReadRootINode();
  if(*path_to_append_to == '/') // first one.
  {
    *path_to_append_to = '\x00';
    path_to_append_to++;
  }

  for(u32 i = 0; i < 12; i++)
  {
      if(d->direct_block_ptr[i] == 0)
      {
        break;
      }

      if(Util::strcharcount(path_to_append_to, '/') == 0) // last one, need to append as entry
      {

        Directory dir;
        Util::memset(&dir, sizeof(Directory), '\x00');
        dir.inode = inode_to_append;
        dir.total_size_of_entry = sizeof(u32) + sizeof(u16) + sizeof(u8) + sizeof(u8) + Util::strlen(path_to_append_to) + 1;
        dir.name_length_least_bits = Util::strlen(path_to_append_to);
        dir.type_indicator = file_type;
        Util::printf("AddNode: %d\n",dir.inode);
        Util::memcopy(path_to_append_to, dir.name_string, Util::strlen(path_to_append_to));
        if(AppendEntry((d->direct_block_ptr[i]), &dir))
        {
          return;
        }
      }
      else // move to next dir
      {
        Directory* tmp = (Directory*)ReadBlock(d->direct_block_ptr[i]);
        INode *file = (INode*)0;
        if((file = FindIfFileInDir(tmp, path_to_append_to)) != (INode*)0) // found dir!
        {
          AppendToDir(file, inode_to_append, (path_to_append_to + Util::strlen(path_to_append_to)), file_type);
        }
      }
  }
}

bool EXT2System::AppendEntry(u32 block_number_of_dir, Directory* append)
{

  Directory* dir = (Directory*)ReadBlock(block_number_of_dir);
  u8* ptr = (u8*)dir;
  u32 real_size = DIRECTORY_PADDING + append->name_length_least_bits;

  u32 counter = 0;

  while(dir->inode != 0)
  {
    Util::printf("[?]%s\n", dir->name_string);
    u32 true_size = dir->name_length_least_bits + DIRECTORY_PADDING;
    true_size += 4 - true_size % 4; //padding

    if(dir->total_size_of_entry != true_size) // last one!
    {
      dir->total_size_of_entry = true_size;
      counter += dir->total_size_of_entry;
      dir = (Directory*)((u8*)dir + dir->total_size_of_entry);
      append->total_size_of_entry = m_size_of_each_block - counter;
      break;
    }
    counter += dir->total_size_of_entry;
    dir = (Directory*)((u8*)dir + dir->total_size_of_entry);

  }

  Util::memcopy(append, dir, append->total_size_of_entry);

  Directory* ptr2 = (Directory*)ptr;
  while(ptr2->inode != 0)
  {
    ptr2 = (Directory*)((u8*)ptr2 + ptr2->total_size_of_entry);
  }

  WriteBlock((u8*)ptr, block_number_of_dir, m_size_of_each_block);
  return true;

}
