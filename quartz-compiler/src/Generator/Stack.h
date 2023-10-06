#ifndef QUARTZ_STACK_H
#define QUARTZ_STACK_H

#include <map>
#include <string>
#include <sstream>
#include <vector>

namespace Quartz {
    class Stack {
    public:
        Stack(std::stringstream* out);

        struct Variable {
            Variable(size_t stackLocation)
                    : StackLocation(stackLocation) {}

            size_t StackLocation;
        };

        void Push(const std::string &reg);
        void Pop(const std::string &reg);

        size_t Size();
        bool Contains(const std::string &name);
        Variable* Get(const std::string &name);

        void Insert(const std::string &name);

        void BeginScope();
        void EndScope();
    private:
        size_t m_StackSize;

        std::map<std::string, Variable> m_Variables;
        std::vector<size_t> m_Scopes;
        std::stringstream* m_Out;
    };
}

#endif //QUARTZ_STACK_H
