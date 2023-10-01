#ifndef QUARTZ_GENERATOR_H
#define QUARTZ_GENERATOR_H

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "Token.h"
#include "ParserNode.h"

class Generator {
public:
    Generator(NodeProgram source);

    std::string GenerateProgram();
private:
    void GenerateExpr(NodeExpr* expr);
    void GenerateStatement(NodeStatement* statement);
    void GenerateTerm(NodeTerm* term);

    void Push(const std::string& reg);
    void Pop(const std::string& reg);

    NodeProgram m_Source;
    std::stringstream m_Out;

    struct Variable {
        Variable(size_t stackLocation)
            : StackLocation(stackLocation) {}

        size_t StackLocation;
    };

    size_t m_StackSize;
    std::unordered_map<std::string, Variable> m_Variables{};
};


#endif //QUARTZ_GENERATOR_H
