#ifndef QUARTZ_MEMORYPOOL_H
#define QUARTZ_MEMORYPOOL_H

#include <cstddef>

namespace Quartz {
    class MemoryPool {
    public:
        MemoryPool(size_t bytes);

        template<typename T>
        T *Allocate() {
            std::byte *offset = m_Offset;
            m_Offset += sizeof(T);
            T *typedPointer = new(offset) T;
            return typedPointer;
        }

        MemoryPool(const MemoryPool &) = delete;

        MemoryPool &operator=(const MemoryPool &) = delete;

        void Init();

        void Free();

    private:
        size_t m_Size;

        std::byte *m_Buffer = nullptr;
        std::byte *m_Offset = nullptr;
    };
}

#endif //QUARTZ_MEMORYPOOL_H
