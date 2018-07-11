//
// Created by lolblat on 07/07/18.
//

#ifndef OROS_EXT2SYSTEM_H
#define OROS_EXT2SYSTEM_H

#endif //OROS_EXT2SYSTEM_H

#include "FileSystemStructs.h"
class EXT2System
{
private:
    SuperBlock m_super_block; // save the super block of the system
    BlockGroupDescriptor m_descriptor;
public:
    EXT2System();
    ~EXT2System();
};