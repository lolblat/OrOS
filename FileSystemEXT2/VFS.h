#ifndef _OS_VFS
#define _OS_VFS
#include "EXT2System.h"
#include "../cpu/types.h"
#include "../MemoryAllocation/Operators.h"
extern "C"
{
  class VFS
  {
  private:
    EXT2System* m_filesystem;
  public:
    VFS();
    u8* ReadFile(u8* file_path);
  };
};
#endif
