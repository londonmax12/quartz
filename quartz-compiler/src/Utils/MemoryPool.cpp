#include "MemoryPool.h"

#include <cstdlib>

namespace Quartz {
    MemoryPool::MemoryPool(size_t bytes)
            : m_Size(bytes)
    {

    }

    bool MemoryPool::Init() {
        if (m_Buffer)
            return false;
        m_Buffer = static_cast<std::byte*>(malloc(m_Size));
        m_Offset = m_Buffer;
        return true;
    }

    void MemoryPool::Free() {
        if (m_Buffer != nullptr) {
            free(m_Buffer);
            m_Buffer = nullptr;
        }
    }

    MemoryPool::~MemoryPool() {
        Free();
    }
}