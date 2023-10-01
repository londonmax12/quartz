#include "MemoryPool.h"

#include <cstdlib>

MemoryPool::MemoryPool(size_t bytes)
   : m_Size(bytes)
{

}

void MemoryPool::Init() {
    m_Buffer = static_cast<std::byte*>(malloc(m_Size));
    m_Offset = m_Buffer;
}

void MemoryPool::Free() {
    if (m_Buffer != nullptr)
        free(m_Buffer);
}