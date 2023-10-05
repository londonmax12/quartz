#include "Stack.h"

namespace Quartz {
    Stack::Stack(std::stringstream* out)
        : m_Out(out)
    {
        m_Variables = std::map<std::string, Variable>();
    }

    void Stack::Push(const std::string& reg) {
        *m_Out << "    push " << reg << "\n";
        m_StackSize++;
    }

    void Stack::Pop(const std::string& reg) {
        *m_Out << "    pop " << reg << "\n";
        m_StackSize--;
    }

    size_t Stack::Size() {
        return m_StackSize;
    }

    bool Stack::Contains(const std::string& name) {
        return m_Variables.find(name) != m_Variables.end();
    }

    Stack::Variable* Stack::Get(const std::string& name) {
        if (Contains(name))
            return &m_Variables.at(name);
        return nullptr;
    }

    void Stack::Insert(const std::string& name) {
        m_Variables.insert({name, Variable(m_StackSize)});
    }

    void Stack::BeginScope() {
        m_Scopes.push_back(m_Variables.size());
    }

    void Stack::EndScope() {
        size_t popCount = m_Variables.size() - m_Scopes.back();
        *m_Out << "    add rsp, " << popCount * 8 << "\n";
        m_StackSize -= popCount;

        for (size_t i = 0; i < popCount; ++i) {
            auto lastElementIterator = m_Variables.end();
            lastElementIterator--;
            m_Variables.erase(lastElementIterator);
        }
        m_Scopes.pop_back();
    }
}

