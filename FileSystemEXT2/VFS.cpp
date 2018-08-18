#include "VFS.h"
VFS::VFS()
{
  m_filesystem = new EXT2System(Primary, Master);
}

u8* VFS::ReadFile(u8* path)
{

  INode* inode = m_filesystem->GetFileByPath(path);
  return m_filesystem->GetContentOfINode(inode);
}
