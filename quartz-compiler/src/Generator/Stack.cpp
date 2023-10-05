#include "Stack.h"

namespace Quartz {
    Stack::Stack()
    {
        m_Variables = std::map<std::string, Variable>();
    }

    void Stack::Push() {
        m_StackSize++;
    }

    void Stack::Pop() {
        m_StackSize--;
    }

    size_t Stack::Size() {
        return m_StackSize;
    }

    bool Stack::Contains(std::string name) {
        return m_Variables.find(name) != m_Variables.end();
    }

    Stack::Variable* Stack::Get(std::string name) {
        if (Contains(name))
            return &m_Variables.at(name);
        return nullptr;
    }

    void Stack::Insert(std::string name) {
        m_Variables.insert({name, Variable(m_StackSize)});
    }
}

