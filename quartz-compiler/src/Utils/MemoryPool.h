#ifndef QUARTZ_MEMORYPOOL_H
#define QUARTZ_MEMORYPOOL_H

#include <stdexcept>

namespace Quartz {
    class MemoryAllocationException : public std::exception {
    public:
        MemoryAllocationException(const std::string& message)
            : errorMessage(message) {}

        const char* what() const noexcept override {
            return errorMessage.c_str();
        }
    private:
        std::string errorMessage;
    };

    class MemoryPool {
    public:
        MemoryPool(size_t bytes);
        ~MemoryPool();

        template<typename T>
        T* Allocate() {
            if (!m_Buffer)
                throw MemoryAllocationException("Allocation called before pool initialization");

            std::byte* offset = m_Offset;
            m_Offset += sizeof(T);
            if (offset > m_Buffer + m_Size)
                throw MemoryAllocationException("Pool ran out of memory");

            T* typedPointer = new(offset) T;
            return typedPointer;
        }

        template<typename T, typename... Args>
        T* Allocate(Args&&... args) {
            if (!m_Buffer)
                throw MemoryAllocationException("Allocation called before pool initialization");

            std::byte* offset = m_Offset;
            m_Offset += sizeof(T);
            if (offset > m_Buffer + m_Size)
                throw MemoryAllocationException("Pool ran out of memory");

            T* typedPointer = new(offset) T(std::forward<Args>(args)...);
            return typedPointer;
        }

        MemoryPool(const MemoryPool &) = delete;
        MemoryPool &operator=(const MemoryPool &) = delete;

        bool Init();
        void Free();
    private:
        size_t m_Size;

        std::byte* m_Buffer = nullptr;
        std::byte* m_Offset = nullptr;
    };
}

#endif //QUARTZ_MEMORYPOOL_H
