#include "Operators.h"
void *operator new(size_t size)
{
   return MemoryManager::GetInstance()->kmalloc(size);
}
