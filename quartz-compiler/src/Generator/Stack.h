#ifndef QUARTZ_STACK_H
#define QUARTZ_STACK_H

#include <map>
#include <string>
#include <sstream>

namespace Quartz {
    class Stack {
    public:
        Stack();

        struct Variable {
            Variable(size_t stackLocation)
                    : StackLocation(stackLocation) {}

            size_t StackLocation;
        };

        void Push();
        void Pop();

        size_t Size();
        bool Contains(std::string name);
        Variable* Get(std::string name);

        void Insert(std::string name);
    private:
        size_t m_StackSize;
        std::map<std::string, Variable> m_Variables;
    };
}

#endif //QUARTZ_STACK_H
